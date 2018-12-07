#include "Engine\Managers\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\ThirdParty\stb_image.h"
#include <Engine\Assets\Mesh.h>
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

		LLGL::SrcImageDescriptor AssetManager::LoadTex_stb_image(const std::string& Path,  LLGL::TextureDescriptor & Desc)
		{
			int req_c;
			switch (Desc.format)
			{
			case LLGL::Format::R8UNorm:
				req_c = 1; break;
			case LLGL::Format::RG8UNorm:
				req_c = 2; break;
			case LLGL::Format::RGB8UNorm:
				req_c = 3; break;
			case LLGL::Format::RGBA8UNorm:
			case LLGL::Format::BGRA8sRGB:
				req_c = 4; break;
			default:					req_c = 4; break;
			}
			LLGL::SrcImageDescriptor Data;

			//stbi_set_flip_vertically_on_load(Desc.FlipY_Axis);
			Data.data = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);

			return Data;
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
				
				LLGL::Texture::Delete(&x.second.mTexture);
			}
			mImportedTextures.clear();
			mHashedTexturesNames.clear();
		}

		Assets::Mesh & AssetManager::Import(const std::string & Path, const MeshLoadingDesc &desc, Assets::Material& material)
		{
			// TODO: insert return statement here
			return LoadMesh_Assimp(Path, material, desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path,  LLGL::TextureDescriptor & Desc)
		{
			return Import(Path, Assets::TextureUsageType::Unknown, Desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const Assets::TextureUsageType & type,  LLGL::TextureDescriptor & Desc)
		{
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

			Data.HashedName = hashedname;

			if (mSaveTextureNames)
			{
				mHashedTexturesNames[hashedname] = Path;
			}

			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) + '\n'));

			Assets::Texture Tex;
			Tex.SetUsageType(type);
			LLGL::Texture::Create(&Tex.mTexture, Data, Desc);

			return mImportedTextures[hashedname] = Tex;
		}
					
		LLGL::SrcImageDescriptor AssetManager::TextureCube_Load(const std::string& Path,  LLGL::TextureDescriptor& Desc)
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
		std::array<LLGL::SrcImageDescriptor, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths,  LLGL::TextureDescriptor& desc)
		{
			LLGL::SrcImageDescriptor data1, data2, data3, data4, data5, data6;
			LLGL::TextureDescriptor Desc = desc;
			//Desc.FlipY_Axis = false;
			data1 = TextureCube_Load(Paths.at(0), Desc);
			data2 = TextureCube_Load(Paths.at(1), Desc);
			data3 = TextureCube_Load(Paths.at(2), Desc);
			data4 = TextureCube_Load(Paths.at(3), Desc);
			data5 = TextureCube_Load(Paths.at(4), Desc);
			data6 = TextureCube_Load(Paths.at(5), Desc);
			
			std::array<LLGL::SrcImageDescriptor, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}