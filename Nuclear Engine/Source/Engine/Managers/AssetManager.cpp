#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine\Audio\AudioEngine.h>
#include <Engine\Assets\DefaultTextures.h>

#define EXPOSE_FREEIMAGE_IMPORTER
#define EXPOSE_ASSIMP_IMPORTER
#include <Engine\Importers\AssimpImporter.h>
#include <Engine\Importers\FreeImageImporter.h>
#include <utility>
#include "CreateTextureFromRawImage.h"
#include <Core\FileSystem.h>

namespace NuclearEngine {
	namespace Managers {
		
	
		AssetManager::AssetManager(AssetManagerDesc desc)
			: mDesc(desc)
		{
			//Initialize Containers
			mImportedTextures = std::unordered_map<Uint32, Assets::Texture>();
			mHashedTexturesPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedMeshes = std::unordered_map<Uint32, Assets::Mesh>();
			mHashedMeshesPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedMaterials = std::unordered_map<Uint32, Assets::Material>();
			mHashedMaterialsPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedAnimations = std::unordered_map<Uint32, Assets::Animations>();
			mHashedAnimationsPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedAudioClips = std::unordered_map<Uint32, Assets::AudioClip>();
			mHashedAudioClipsPaths = std::unordered_map<Uint32, Core::Path>();

			mTextureImporter = Importers::TextureImporterDelegate::create<&Importers::FreeImageLoad>();
			mMeshImporter = Importers::MeshImporterDelegate::create<&Importers::AssimpLoadMesh>();
		}

		AssetManager::~AssetManager()
		{
			FlushContainers(mDesc.mFlushFlagOnShutdown);
		}

		void AssetManager::FlushContainers(ASSET_MANAGER_FLUSH_FLAGS flag)
		{
			/*for (auto& it : mImportedMaterials) {
				 Do stuff

				for (auto& it1 : it.second.mPixelShaderTextures) {
					for (auto& it2 : it1.mData) {
						it2.mTex.mTextureView.RawPtr()->Release

						it2.mTex.mTextureView.Release();
					}
				}
				it.second.mTextureView.Release();
			}*/
		//
			mImportedTextures.clear();
			mHashedTexturesPaths.clear();

			mImportedMeshes.clear();
			mHashedMeshesPaths.clear();
		}

		Assets::Texture AssetManager::Import(const Core::Path & Path, const Importers::TextureLoadingDesc & Desc)
		{
			return Import(Path, Assets::TextureUsageType::Unknown, Desc);
		}

		Assets::Texture AssetManager::Import(const Core::Path & Path, const Assets::TextureUsageType & type, const Importers::TextureLoadingDesc & Desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			auto it = mImportedTextures.find(hashedname);
			if (it != mImportedTextures.end())
			{
				return it->second;
			}

			auto Data = mTextureImporter(Path.mRealPath, Desc);

			if (Data.mData == NULL)
			{
				Log.Error(std::string("[AssetManager : " + mDesc.mName +  "] Failed To Load Texture: " + Path.mInputPath + " Hash: " + Utilities::int_to_hex<Uint32>(hashedname) + '\n'));
				return Assets::DefaultTextures::DefaultBlackTex;
			}


			if (mSaveTexturesPaths)
			{
				mHashedTexturesPaths[hashedname] = Path;
			}

			Log.Info(std::string("[AssetManager : " + mDesc.mName +  "] Loaded Texture: " + Path.mInputPath + " Hash: " + Utilities::int_to_hex<Uint32>(hashedname) + '\n'));

			Assets::Texture Tex;
			Internal::CreateTextureFromRawImage(Data, Desc, &Tex);
			Tex.SetName(hashedname);
			Tex.SetUsageType(type);

			return mImportedTextures[hashedname] = Tex;
		}

		Assets::Texture AssetManager::Import(const Assets::Image & Image, const Importers::TextureLoadingDesc & Desc)
		{
			Assets::Texture Tex;
			Internal::CreateTextureFromRawImage(Image, Desc, &Tex);
			return Tex;
		}

		Assets::AudioClip* AssetManager::Import(const Core::Path& Path, AUDIO_IMPORT_MODE mode)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			Log.Info("[AssetManager : " + mDesc.mName + "] Loading: " + Path.mInputPath +" Hash: " + Utilities::int_to_hex<Uint32>(hashedname) + '\n');


			mImportedAudioClips[hashedname] = Assets::AudioClip();
			auto result = &mImportedAudioClips[hashedname];
			Audio::AudioEngine::GetSystem()->createSound(Path.mRealPath.c_str(), mode, 0, &result->mSound);

			return result;
		}


		std::tuple<Assets::Mesh*, Assets::Material*, Assets::Animations*> AssetManager::Import(const Core::Path& Path, const Importers::MeshLoadingDesc& desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			Assets::Animations Animation;
			Assets::Animations* anim = nullptr;
		
			mImportedMeshes[hashedname] = Assets::Mesh();
			mImportedMaterials[hashedname] = Assets::Material();

		
			Assets::Mesh* Mesh = &mImportedMeshes[hashedname];
			Assets::Material* Material = &mImportedMaterials[hashedname];




			if (!mMeshImporter({ Path.mRealPath.c_str(), desc, this}, Mesh, Material, &Animation))
			{
				Log.Error("[AssetManager : " + mDesc.mName + "] Loading Model: " + Path.mInputPath + "\n");
				return { Mesh , Material, anim };
			}

			if (desc.LoadAnimation)
			{
				if (Animation.isValid == true)
				{
					mImportedAnimations[hashedname] = Animation;
					anim = &mImportedAnimations[hashedname];
					if (mSaveAnimationsPaths)
						mHashedAnimationsPaths[hashedname] = Path;
				}
			}

			Log.Info("[AssetManager : " + mDesc.mName + "] Loaded Model: " + Path.mInputPath + "\n");

			Mesh->Create();
			
			if (mSaveMeshesPaths)
				mHashedMeshesPaths[hashedname] = Path;

			if (mSaveMaterialsPaths)
				mHashedMaterialsPaths[hashedname] = Path;


			return { Mesh , Material, anim };
		}

					
		Assets::Image AssetManager::TextureCube_Load(const Core::Path& Path, const Importers::TextureLoadingDesc& Desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			auto Data = mTextureImporter(Path.mRealPath, Desc);

			if (Data.mData == NULL)
			{
				Log.Error(std::string("[AssetManager : " + mDesc.mName +  "] Failed To Load Texture2D (For CubeMap): " + Path.mInputPath + '\n'));
				return Data;
			}
	
			Log.Info(std::string("[AssetManager : " + mDesc.mName +  "] Loaded Texture2D (For CubeMap): " + Path.mInputPath + '\n'));

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
		std::array<Assets::Image, 6> AssetManager::LoadTextureCubeFromFile(const std::array<Core::Path, 6>& Paths, const Importers::TextureLoadingDesc& desc)
		{
			Assets::Image data1, data2, data3, data4, data5, data6;
			Importers::TextureLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;
			data1 = TextureCube_Load(Paths.at(0), Desc);
			data2 = TextureCube_Load(Paths.at(1), Desc);
			data3 = TextureCube_Load(Paths.at(2), Desc);
			data4 = TextureCube_Load(Paths.at(3), Desc);
			data5 = TextureCube_Load(Paths.at(4), Desc);
			data6 = TextureCube_Load(Paths.at(5), Desc);
			
			std::array<Assets::Image, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}

		Assets::Script& AssetManager::ImportScript(const Core::Path& Path)
		{
			Assets::Script result;

			//result.ScriptCode = Core::FileSystem::LoadFileToString(Path);

			return result;
		}
	}
}