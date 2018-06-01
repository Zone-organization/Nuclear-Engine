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
		Graphics::API::Texture_Data AssetManager::LoadTex_stb_image(const std::string& Path, const Graphics::API::Texture_Desc & Desc)
		{
			int req_c;
			switch (Desc.Format)
			{
			case Graphics::API::Format::R8_UNORM:
				req_c = 1; break;
			case Graphics::API::Format::R8G8_UNORM:
				req_c = 2; break;
			case Graphics::API::Format::R8G8B8_UNORM:
				req_c = 3; break;
			case Graphics::API::Format::R8G8B8A8_UNORM:
			case Graphics::API::Format::R8G8B8A8_UNORM_SRGB:
				req_c = 4; break;
			default:					req_c = 4; break;
			}
			Graphics::API::Texture_Data Data;

			stbi_set_flip_vertically_on_load(Desc.FlipY_Axis);
			Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);

			if (Data.Img_Data_Buf == NULL)
				Data.Valid = false;
			else
				Data.Valid = true;

			return Data;
		}
		
		Assets::Mesh & AssetManager::LoadMesh_Assimp(const std::string & Path, const Managers::MeshLoadingDesc & desc)
		{
			Internal::AssimpImporter importer;
			return importer.Load(Path, desc);
			// TODO: insert return statement here
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
				Graphics::API::Texture::Delete(&x.second.mTexture);
			}
			mImportedTextures.clear();
			mHashedTexturesNames.clear();
		}
			
		Assets::Mesh & AssetManager::Import(const std::string & Path, const MeshLoadingDesc &desc)
		{
			// TODO: insert return statement here
			return LoadMesh_Assimp(Path,desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const Graphics::API::Texture_Desc & Desc)
		{
			return Import(Path, Assets::TextureUsageType::Unknown, Desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const Assets::TextureUsageType & type, const Graphics::API::Texture_Desc & Desc)
		{
			auto hashedname = Utilities::Hash(Path);

			auto it = mImportedTextures.find(hashedname);
			if (it != mImportedTextures.end())
			{
				return it->second;
			}

			auto Data = DefaultTextureImporter(Path, Desc);

			if (!Data.Valid)
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
			Graphics::API::Texture::Create(&Tex.mTexture, Data, Desc);

			return mImportedTextures[hashedname] = Tex;
		}
					
		Graphics::API::Texture_Data AssetManager::TextureCube_Load(const std::string& Path, const Graphics::API::Texture_Desc& Desc)
		{
			auto hashedname = Utilities::Hash(Path);

			auto Data = DefaultTextureImporter(Path, Desc);

			if (!Data.Valid)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture2D (For CubeMap): " + Path + '\n'));
				return Data;
			}

			Data.HashedName = hashedname;
	
			Log.Info(std::string("[AssetManager] Loaded Texture2D (For CubeMap): " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) +'\n'));

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
		std::array<Graphics::API::Texture_Data, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const Graphics::API::Texture_Desc& desc)
		{
			Graphics::API::Texture_Data data1, data2, data3, data4, data5, data6;
			Graphics::API::Texture_Desc Desc = desc;
			Desc.FlipY_Axis = false;
			data1 = TextureCube_Load(Paths.at(0), Desc);
			data2 = TextureCube_Load(Paths.at(1), Desc);
			data3 = TextureCube_Load(Paths.at(2), Desc);
			data4 = TextureCube_Load(Paths.at(3), Desc);
			data5 = TextureCube_Load(Paths.at(4), Desc);
			data6 = TextureCube_Load(Paths.at(5), Desc);
			
			std::array<Graphics::API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}