#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>

#define EXPOSE_STB_IMAGE_IMPORTER
#define EXPOSE_ASSIMP_IMPORTER
#include <Engine\Importers\AssimpImporter.h>
#include <Engine\Importers\StbImageImporter.h>

#include "CreateTextureFromRawImage.h"

namespace NuclearEngine {
	namespace Managers {
		
		template< typename T >
		std::string int_to_hex(T i)
		{
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(T) * 2)
				<< std::hex << i;
			return stream.str();
		}
		AssetManager::AssetManager(AssetManagerDesc desc)
			: mDesc(desc)
		{
			//Initialize Containers
			mImportedTextures = std::unordered_map<Uint32, Assets::Texture>();
			mHashedTexturesPaths = std::unordered_map<Uint32, std::string>();

			mImportedMeshes = std::unordered_map<Uint32, Assets::Mesh>();
			mHashedMeshesPaths = std::unordered_map<Uint32, std::string>();

			mImportedMaterials = std::unordered_map<Uint32, Assets::Material>();
			mHashedMaterialsPaths = std::unordered_map<Uint32, std::string>();


			mTextureImporter = TextureImport::create<&Importers::StbImageLoad>();
			mMeshImporter = MeshImport::create<&Importers::AssimpLoadMesh>();
		}

		AssetManager::~AssetManager()
		{
			FlushContainers(mDesc.mFlushFlagOnShutdown);
		}

		void AssetManager::FlushContainers(ASSET_MANAGER_FLUSH_FLAGS flag)
		{
			//TODO
			for (auto x : mImportedTextures)
			{
				x.second.mTexture.Release();
			}
			mImportedTextures.clear();
			//mHashedTexturesNames.clear();
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const TextureLoadingDesc & Desc)
		{
			return Import(Path, Assets::TextureUsageType::Unknown, Desc);
		}

		Assets::Texture & AssetManager::Import(const std::string & Path, const Assets::TextureUsageType & type, const TextureLoadingDesc & Desc)
		{
			auto hashedname = Utilities::Hash(Path);

			auto it = mImportedTextures.find(hashedname);
			if (it != mImportedTextures.end())
			{
				return it->second;
			}

			auto Data = mTextureImporter(Path, Desc);

			if (Data.mData == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture: " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) + '\n'));
				return Assets::Texture();
			}


			if (mSaveTexturesPaths)
			{
				mHashedTexturesPaths[hashedname] = Path;
			}

			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + " Hash: " + int_to_hex<Uint32>(hashedname) + '\n'));

			auto Tex = Internal::CreateTextureFromRawImage(Data, Desc);
			Tex.SetName(hashedname);
			Tex.SetUsageType(type);

			return mImportedTextures[hashedname] = Tex;
		}

		Assets::Texture & AssetManager::Import(const Assets::Image & Image, const TextureLoadingDesc & Desc)
		{
			return Internal::CreateTextureFromRawImage(Image, Desc);
		}

		std::tuple<Assets::Mesh, Assets::Material> AssetManager::Import(const std::string & Path, const Managers::MeshLoadingDesc & desc)
		{
			auto[mesh, material] = mMeshImporter(Path, desc);
			auto hashedname = Utilities::Hash(Path);

			Log.Info("[AssetManager] Loaded Mesh & Material at: " + Path + "\n");
			mImportedMaterials[hashedname] = material;
			mImportedMeshes[hashedname] = mesh;

			if (mSaveMeshesPaths)
				mHashedMeshesPaths[hashedname] = Path;

			if (mSaveMaterialsPaths)
				mHashedMaterialsPaths[hashedname] = Path;

			return { mesh , material };
		}
					
		ITexture * AssetManager::TextureCube_Load(const std::string& Path, const TextureLoadingDesc& Desc)
		{
			auto hashedname = Utilities::Hash(Path);

			auto Data = mTextureImporter(Path, Desc);

			/*if (Data.mData == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture2D (For CubeMap): " + Path + '\n'));
				return Data;
			}
	
			Log.Info(std::string("[AssetManager] Loaded Texture2D (For CubeMap): " + Path + '\n'));

			return Data;*/

			return nullptr;
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
		std::array< ITexture *, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const TextureLoadingDesc& desc)
		{
			ITexture *data1, *data2, *data3, *data4, *data5, *data6;
			TextureLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;
			data1 = TextureCube_Load(Paths.at(0), Desc);
			data2 = TextureCube_Load(Paths.at(1), Desc);
			data3 = TextureCube_Load(Paths.at(2), Desc);
			data4 = TextureCube_Load(Paths.at(3), Desc);
			data5 = TextureCube_Load(Paths.at(4), Desc);
			data6 = TextureCube_Load(Paths.at(5), Desc);
			
			std::array<ITexture *, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}