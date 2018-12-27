#include "Engine\Managers\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\ThirdParty\stb_image.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\Mesh.h>
#include "Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.h"
#include "AssimpImporter.h"
#include <Base\Utilities\Hash.h>
#include <sstream>
#include <iomanip>

namespace NuclearEngine {
	namespace Managers {
		std::unordered_map<Uint32,Assets::Texture> AssetManager::mImportedTextures = std::unordered_map<Uint32, Assets::Texture>();
		std::unordered_map<Uint32, std::string> AssetManager::mHashedTexturesNames = std::unordered_map<Uint32, std::string>();

		std::unordered_map<Uint32, Assets::Mesh> AssetManager::mImportedMeshes = std::unordered_map<Uint32, Assets::Mesh>();
		std::unordered_map<Uint32, std::string> AssetManager::mHashedMeshesNames = std::unordered_map<Uint32, std::string>();

		std::unordered_map<Uint32, Assets::Material> AssetManager::mImportedMaterials = std::unordered_map<Uint32, Assets::Material>();
		std::unordered_map<Uint32, std::string> AssetManager::mHashedMaterialsNames = std::unordered_map<Uint32, std::string>();


		static const float a = 0.055f;

		// https://en.wikipedia.org/wiki/SRGB
		float SRGBToLinear(float SRGB)
		{
			if (SRGB < 0.04045f)
				return SRGB / 12.92f;
			else
				return pow((SRGB + a) / (1 + a), 2.4f);
		}

		float LinearToSRGB(float c)
		{
			if (c < 0.0031308f)
				return 12.92f * c;
			else
				return (1 + a) * pow(c, 1.f / 2.4f) - a;
		}

		template<typename ChannelType>
		ChannelType SRGBAverage(ChannelType c0, ChannelType c1, ChannelType c2, ChannelType c3)
		{
			constexpr float NormVal = static_cast<float>(std::numeric_limits<ChannelType>::max());
			float fc0 = static_cast<float>(c0) / NormVal;
			float fc1 = static_cast<float>(c1) / NormVal;
			float fc2 = static_cast<float>(c2) / NormVal;
			float fc3 = static_cast<float>(c3) / NormVal;

			float fLinearAverage = (SRGBToLinear(fc0) + SRGBToLinear(fc1) + SRGBToLinear(fc2) + SRGBToLinear(fc3)) / 4.f;
			float fSRGBAverage = LinearToSRGB(fLinearAverage);
			Int32 SRGBAverage = static_cast<Int32>(fSRGBAverage * NormVal);
			SRGBAverage = std::min(SRGBAverage, static_cast<Int32>(std::numeric_limits<ChannelType>::max()));
			SRGBAverage = std::max(SRGBAverage, static_cast<Int32>(std::numeric_limits<ChannelType>::min()));
			return static_cast<ChannelType>(SRGBAverage);
		}

		template < typename ChannelType >
		void ComputeCoarseMip(Uint32 NumChannels, bool IsSRGB,
			const void* pFineMip,
			Uint32      FineMipStride,
			void*       pCoarseMip,
			Uint32      CoarseMipStride,
			Uint32      CoarseMipWidth,
			Uint32      CoarseMipHeight)
		{
			for (size_t row = 0; row < size_t{ CoarseMipHeight }; ++row)
				for (size_t col = 0; col < size_t{ CoarseMipWidth }; ++col)
				{
					auto FineRow0 = reinterpret_cast<const ChannelType*>(reinterpret_cast<const Uint8*>(pFineMip) + row * 2 * size_t{ FineMipStride });
					auto FineRow1 = reinterpret_cast<const ChannelType*>(reinterpret_cast<const Uint8*>(pFineMip) + (row * 2 + 1) * size_t { FineMipStride });

					for (Uint32 c = 0; c < NumChannels; ++c)
					{
						auto Col00 = FineRow0[col * 2 * NumChannels + c];
						auto Col01 = FineRow0[(col * 2 + 1) * NumChannels + c];
						auto Col10 = FineRow1[col * 2 * NumChannels + c];
						auto Col11 = FineRow1[(col * 2 + 1) * NumChannels + c];
						auto &DstCol = reinterpret_cast<ChannelType*>(reinterpret_cast<Uint8*>(pCoarseMip) + row * size_t{ CoarseMipStride })[col * NumChannels + c];
						if (IsSRGB)
							DstCol = SRGBAverage(Col00, Col01, Col10, Col11);
						else
							DstCol = (Col00 + Col01 + Col10 + Col11) / 4;
					}
				}
		}

		template < typename ChannelType >
		void RGBToRGBA(const void*  pRGBData,
			Uint32       RGBStride,
			void*        pRGBAData,
			Uint32       RGBAStride,
			Uint32       Width,
			Uint32       Height)
		{
			for (size_t row = 0; row < size_t{ Height }; ++row)
				for (size_t col = 0; col < size_t{ Width }; ++col)
				{
					for (int c = 0; c < 3; ++c)
					{
						reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pRGBAData) + size_t{ RGBAStride } *row))[col * 4 + c] =
							reinterpret_cast<const ChannelType*>((reinterpret_cast<const Uint8*>(pRGBData) + size_t{ RGBStride } *row))[col * 3 + c];
					}
					reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pRGBAData) + size_t{ RGBAStride } *row))[col * 4 + 3] = std::numeric_limits<ChannelType>::max();
				}
		}

		Assets::Image& AssetManager::LoadTex_STB(const std::string& Path, const TextureLoadingDesc & Desc)
		{
			int ReqNumComponents, Channels;
			switch (Desc.mFormat)
			{
			case Diligent::TEX_FORMAT_R8_UNORM:
				ReqNumComponents = 1;
				Channels = 8;
				break;
			case Diligent::TEX_FORMAT_RG8_UNORM:
				ReqNumComponents = 2;
				Channels = 8;
				break;
			/*case Diligent::TEX_FORMAT_RGB8_UINT:
				NumComponents = 3; break;*/
			case Diligent::TEX_FORMAT_RGBA8_UNORM:
			case Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB:
				ReqNumComponents = 4;
				Channels = 8;
				break;
			default: 
				ReqNumComponents = 4;
				Channels = 8;
				break;
			}

			int texWidth = 0, texHeight = 0, texComponents = 0, BitsPerPixel = 0;
			stbi_set_flip_vertically_on_load(Desc.mFlipY_Axis);
			auto data = stbi_load(Path.c_str(), &texWidth, &texHeight, &texComponents, ReqNumComponents);
			BitsPerPixel = Channels * texComponents;
			using namespace Diligent;

			Diligent::TextureDesc TexDesc;
			TexDesc.Name = Desc.mName.c_str();
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = texWidth;
			TexDesc.Height = texHeight;
			TexDesc.MipLevels = ComputeMipLevelsCount(TexDesc.Width, TexDesc.Height);
			if (Desc.mMipLevels > 0)
				TexDesc.MipLevels = std::min(TexDesc.MipLevels, Desc.mMipLevels);
			TexDesc.Usage = Desc.mUsage;
			TexDesc.BindFlags = Desc.mBindFlags;
			TexDesc.Format = Desc.mFormat;
			TexDesc.CPUAccessFlags = Desc.mCPUAccessFlags;

			auto ChannelDepth = BitsPerPixel / texComponents;
			Uint32 NumComponents = texComponents == 3 ? 4 : texComponents;

			bool IsSRGB = (texComponents >= 3 && ChannelDepth == 8) ? Desc.mIsSRGB : false;
			if (TexDesc.Format == TEX_FORMAT_UNKNOWN)
			{
				if (ChannelDepth == 8)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R8_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
					case 4: TexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", texComponents, ")");
					}
				}
				else if (ChannelDepth == 16)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R16_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
					case 4: TexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", texComponents, ")");
					}
				}
				else
					LOG_ERROR_AND_THROW("Unsupported color channel depth (", ChannelDepth, ")");
			}
			else
			{
				const auto& TexFmtDesc = GetTextureFormatAttribs(TexDesc.Format);
				if (TexFmtDesc.NumComponents != NumComponents)
					LOG_ERROR_AND_THROW("Incorrect number of components ", texComponents, ") for texture format ", TexFmtDesc.Name);
				if (TexFmtDesc.ComponentSize != ChannelDepth / 8)
					LOG_ERROR_AND_THROW("Incorrect channel size ", ChannelDepth, ") for texture format ", TexFmtDesc.Name);
			}


			std::vector<TextureSubResData> pSubResources(TexDesc.MipLevels);
			std::vector< std::vector<Uint8> > Mips(TexDesc.MipLevels);

			if (texComponents == 3)
			{
				VERIFY_EXPR(NumComponents == 4);
				auto RGBAStride = texWidth * NumComponents * ChannelDepth / 8;
				RGBAStride = (RGBAStride + 3) & (-4);
				Mips[0].resize(size_t{ RGBAStride } *size_t{ texHeight });
				pSubResources[0].pData = Mips[0].data();
				pSubResources[0].Stride = RGBAStride;
				if (ChannelDepth == 8)
					RGBToRGBA<Uint8>(data, ImgDesc.RowStride,
						Mips[0].data(), RGBAStride,
						ImgDesc.Width, ImgDesc.Height);
				else if (ChannelDepth == 16)
					RGBToRGBA<Uint16>(data, ImgDesc.RowStride,
						Mips[0].data(), RGBAStride,
						ImgDesc.Width, ImgDesc.Height);
			}
			else
			{
				pSubResources[0].pData = data;
				pSubResources[0].Stride = ImgDesc.RowStride;
			}

			auto MipWidth = TexDesc.Width;
			auto MipHeight = TexDesc.Height;
			for (Uint32 m = 1; m < TexDesc.MipLevels; ++m)
			{
				auto CoarseMipWidth = std::max(MipWidth / 2u, 1u);
				auto CoarseMipHeight = std::max(MipHeight / 2u, 1u);
				auto CoarseMipStride = CoarseMipWidth * NumComponents * ChannelDepth / 8;
				CoarseMipStride = (CoarseMipStride + 3) & (-4);
				Mips[m].resize(size_t{ CoarseMipStride } *size_t{ CoarseMipHeight });

				if (Desc.mGenerateMips)
				{
					if (ChannelDepth == 8)
						ComputeCoarseMip<Uint8>(NumComponents, IsSRGB,
							pSubResources[m - 1].pData, pSubResources[m - 1].Stride,
							Mips[m].data(), CoarseMipStride,
							CoarseMipWidth, CoarseMipHeight);
					else if (ChannelDepth == 16)
						ComputeCoarseMip<Uint16>(NumComponents, IsSRGB,
							pSubResources[m - 1].pData, pSubResources[m - 1].Stride,
							Mips[m].data(), CoarseMipStride,
							CoarseMipWidth, CoarseMipHeight);
				}

				pSubResources[m].pData = Mips[m].data();
				pSubResources[m].Stride = CoarseMipStride;

				MipWidth = CoarseMipWidth;
				MipHeight = CoarseMipHeight;
			}

			TextureData TexData;
			TexData.pSubResources = pSubResources.data();
			TexData.NumSubresources = TexDesc.MipLevels;

			Diligent::ITexture *Tex;
			Graphics::Context::GetDevice()->CreateTexture(TexDesc, TexData, &Tex);

			return Tex;
		}
		
		Assets::Mesh & AssetManager::LoadMesh_Assimp(const std::string & Path, Assets::Material& material, const Managers::MeshLoadingDesc & desc)
		{
			Internal::AssimpImporter importer;
			return importer.Load(Path, desc, material);
		}

		TextureImport AssetManager::DefaultTextureImporter = TextureImport::create<&AssetManager::LoadTex_stb_image>();
		bool AssetManager::mSaveTextureNames = false;

		template< typename T >
		std::string int_to_hex(T i)
		{
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(T) * 2)
				<< std::hex << i;
			return stream.str();
		}
		void AssetManager::Initialize(bool SaveTextureNames)
		{
			mSaveTextureNames = SaveTextureNames;
		}

		void AssetManager::ShutDown()
		{
			for (auto x : mImportedTextures)
			{
				x.second.mTexture.Release();
			}
			mImportedTextures.clear();
			mHashedTexturesNames.clear();
		}

		Assets::Mesh & AssetManager::Import(const std::string & Path, const MeshLoadingDesc &desc, Assets::Material& material)
		{
			// TODO: insert return statement here
			return LoadMesh_Assimp(Path, material, desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const TextureLoadingDesc & Desc)
		{
			return Import(Path, Assets::TextureUsageType::Unknown, Desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const Assets::TextureUsageType & type, const TextureLoadingDesc & Desc)
		{
			Assets::Texture Tex;

			auto hashedname = Utilities::Hash(Path);

			auto it = mImportedTextures.find(hashedname);
			if (it != mImportedTextures.end())
			{
				return it->second;
			}

			auto Data = DefaultTextureImporter(Path, Desc);

			if (Data.data == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture: " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) + '\n'));
				return Assets::Texture();
			}

			Tex.SetName(hashedname);

			if (mSaveTextureNames)
			{
				mHashedTexturesNames[hashedname] = Path;
			}

			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) + '\n'));

			Tex.SetUsageType(type);

			Tex.mTexture = Graphics::Context::GetRenderer()->CreateTexture(Desc, &Data);

			return mImportedTextures[hashedname] = Tex;
		}
					
		 Diligent::ITexture * AssetManager::TextureCube_Load(const std::string& Path, const TextureLoadingDesc& Desc)
		{
			auto hashedname = Utilities::Hash(Path);

			auto Data = DefaultTextureImporter(Path, Desc);

			if (Data.data == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture2D (For CubeMap): " + Path + '\n'));
				return Data;
			}
	
			Log.Info(std::string("[AssetManager] Loaded Texture2D (For CubeMap): " + Path + '\n'));

			return Data;
		}
		bool AssetManager::DoesTextureExist(Uint32 hashedname, Assets::Texture* texture)
		{
			//Check if Texture has been loaded before
			auto it = mImportedTextures.find(hashedname);
			if (it != mImportedTextures.end())
			{
				texture = &it->second;
				return true;
			}
			texture = nullptr;
			return false;
		}
		std::array< Diligent::ITexture *, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const TextureLoadingDesc& desc)
		{
			Diligent::ITexture *data1, *data2, *data3, *data4, *data5, *data6;
			TextureLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;
			data1 = TextureCube_Load(Paths.at(0), Desc);
			data2 = TextureCube_Load(Paths.at(1), Desc);
			data3 = TextureCube_Load(Paths.at(2), Desc);
			data4 = TextureCube_Load(Paths.at(3), Desc);
			data5 = TextureCube_Load(Paths.at(4), Desc);
			data6 = TextureCube_Load(Paths.at(5), Desc);
			
			std::array<Diligent::ITexture *, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}