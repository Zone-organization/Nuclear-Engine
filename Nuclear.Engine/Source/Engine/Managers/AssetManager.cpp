#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine\Audio\AudioEngine.h>
#include "Engine\Graphics\GraphicsEngine.h"
#include <Core\Logger.h>


#define EXPOSE_FREEIMAGE_IMPORTER
#define EXPOSE_ASSIMP_IMPORTER
#include <Engine\Importers\AssimpImporter.h>
#include <Engine\Importers\FreeImageImporter.h>
#include <utility>
#include <Core\FileSystem.h>

#include <Engine\ECS\Scene.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <Engine\Components\Components.h>

namespace Nuclear {
	namespace Managers {

		Graphics::Texture AssetManager::DefaultBlackTex;
		Graphics::Texture AssetManager::DefaultGreyTex;
		Graphics::Texture AssetManager::DefaultWhiteTex;

		Graphics::Texture AssetManager::DefaultDiffuseTex;
		Graphics::Texture AssetManager::DefaultSpecularTex;
		Graphics::Texture AssetManager::DefaultNormalTex;
	
		AssetManager::AssetManager(AssetManagerDesc desc)
			: mDesc(desc)
		{
			//Initialize Containers
			mImportedImages = std::unordered_map<Uint32, Assets::Image>();
			mHashedImagesPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedMeshes = std::unordered_map<Uint32, Assets::Mesh>();
			mHashedMeshesPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedMaterials = std::unordered_map<Uint32, Assets::Material>();
			mHashedMaterialsPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedAnimations = std::unordered_map<Uint32, Assets::Animations>();
			mHashedAnimationsPaths = std::unordered_map<Uint32, Core::Path>();

			mImportedAudioClips = std::unordered_map<Uint32, Assets::AudioClip>();
			mHashedAudioClipsPaths = std::unordered_map<Uint32, Core::Path>();

			mImageImporter = Importers::ImageImporterDelegate::create<&Importers::FreeImageLoad>();
			mMeshImporter = Importers::MeshImporterDelegate::create<&Importers::AssimpLoadMesh>();
		}

		AssetManager::~AssetManager()
		{
		//	FlushContainers(mDesc.mFlushFlagOnShutdown);
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
			mImportedMeshes.clear();
			mHashedMeshesPaths.clear();

			mImportedImages.clear();
			mHashedImagesPaths.clear();

			mImportedMaterials.clear();
			mHashedMaterialsPaths.clear();

			mImportedAnimations.clear();
			mHashedAnimationsPaths.clear();

			mImportedAudioClips.clear();
			mHashedAudioClipsPaths.clear();
		}

		void AssetManager::Initialize(AssetManagerDesc desc)
		{
			mDesc = desc;
			DefaultBlackTex = Import("Assets/NuclearEngine/DefaultTextures/Black32x32.png", Importers::ImageLoadingDesc());
			DefaultGreyTex = Import("Assets/NuclearEngine/DefaultTextures/Grey32x32.png", Importers::ImageLoadingDesc());
			DefaultWhiteTex = Import("Assets/NuclearEngine/DefaultTextures/White32x32.png", Importers::ImageLoadingDesc());

			DefaultDiffuseTex = DefaultGreyTex;
			DefaultSpecularTex = DefaultWhiteTex;
			DefaultNormalTex = DefaultBlackTex;

			DefaultGreyTex.SetUsageType(Graphics::TextureUsageType::Diffuse);
			DefaultDiffuseTex.SetUsageType(Graphics::TextureUsageType::Diffuse);

			DefaultWhiteTex.SetUsageType(Graphics::TextureUsageType::Diffuse);
			DefaultSpecularTex.SetUsageType(Graphics::TextureUsageType::Specular);

			DefaultNormalTex.SetUsageType(Graphics::TextureUsageType::Normal);
		}

		Graphics::Texture AssetManager::Import(const Core::Path & Path, const Importers::ImageLoadingDesc& Desc, const Graphics::TextureUsageType& type)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			Graphics::Texture result;

			//Check if exists
			auto it = mImportedImages.find(hashedname);
			if (it != mImportedImages.end())
			{
				result.SetImage(&it->second);
				return result;
			}


			//Load
			Assets::ImageData imagedata = mImageImporter(Path.mRealPath, Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager : '{0}'] Failed To Load Texture: '{1}' Hash: '{2}'" , mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
				return DefaultBlackTex;
			}

			//Create
			Assets::Image image(imagedata, Desc);
			
			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager : '{0}'] Failed To Create Texture: '{1}' Hash: '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

				return DefaultBlackTex;
			}
			image.mData.mData = NULL;

			if (mSaveTexturesPaths)
			{
				mHashedImagesPaths[hashedname] = Path;
			}

			image.SetName(hashedname);
			mImportedImages[hashedname] = image;

			result.SetImage(&mImportedImages[hashedname]);
			result.SetUsageType(type);

			NUCLEAR_INFO("[AssetManager : '{0}'] Loaded Texture: '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
		
			return result;
		}

		Graphics::Texture AssetManager::Import(const Assets::ImageData& imagedata, const Importers::ImageLoadingDesc& Desc)
		{
			assert(Desc.mPath != std::string(""));

			Graphics::Texture result;
			auto hashedname = Utilities::Hash(Desc.mPath);

			//Create
			Assets::Image image(imagedata, Desc);

			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager : '{0}'] Failed To Create Texture: '{1}' Hash: '{2}'", mDesc.mName, Desc.mPath, Utilities::int_to_hex<Uint32>(hashedname));
				return DefaultBlackTex;
			}
			image.mData.mData = NULL;
			mImportedImages[hashedname] = image;
			result.SetImage(&mImportedImages[hashedname]);

			NUCLEAR_INFO("[AssetManager : '{0}'] Imported Texture: '{1}' : '{2}'", mDesc.mName, Desc.mPath, Utilities::int_to_hex<Uint32>(hashedname));

			return result;
		}

		//Assets::Image* AssetManager::Import(const Assets::ImageData& Image , const Importers::ImageLoadingDesc & Desc)
		//{
		//	//if (Desc.mPath != std::string(""))
		//	//{
		//	//	auto hashedname = Utilities::Hash(Desc.mPath);

		//	//	
		//	//	Internal::CreateTextureViewFromRawImage(Image, Desc, &mImportedImages[hashedname]);

		//	//}

		//	if (AddToLibrary)
		//	{

		//	}

		//	Assets::Texture Tex;
		//	Internal::CreateTextureViewFromRawImage(Image, Desc, &Tex);
		//	return Tex;
		//}

		Assets::AudioClip* AssetManager::Import(const Core::Path& Path, AUDIO_IMPORT_MODE mode)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			mImportedAudioClips[hashedname] = Assets::AudioClip();
			auto result = &mImportedAudioClips[hashedname];
			Audio::AudioEngine::GetSystem()->createSound(Path.mRealPath.c_str(), mode, 0, &result->mSound);

			NUCLEAR_INFO("[AssetManager : '{0}'] Imported AudioClip: '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			return result;
		}


		std::tuple<Assets::Mesh*, Assets::Material*, Assets::Animations*> AssetManager::Import(const Core::Path& Path, const Importers::MeshLoadingDesc& desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			//Check if exists
			auto itmesh = mImportedMeshes.find(hashedname);
			if (itmesh != mImportedMeshes.end())
			{
				Assets::Animations* anim = nullptr;
				auto itanim = mImportedAnimations.find(hashedname);
				if (itanim != mImportedAnimations.end())
				{
					anim = &itanim->second;
				}

				return { &itmesh->second, &mImportedMaterials[hashedname], anim };
			}

			Assets::Animations Animation;
			Assets::Animations* anim = nullptr;
		
			mImportedMeshes[hashedname] = Assets::Mesh();
			Assets::Mesh* Mesh = &mImportedMeshes[hashedname];
			Assets::Material* Material = nullptr;
			if (desc.LoadMaterial)
			{
				mImportedMaterials[hashedname] = Assets::Material();
				Material = &mImportedMaterials[hashedname];
			}
			if (!mMeshImporter({ Path.mRealPath.c_str(), desc, this}, Mesh, Material, &Animation))
			{
				NUCLEAR_ERROR("[AssetManager : '{0}'] Failed to import Model : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

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
			NUCLEAR_INFO("[AssetManager : '{0}'] Imported Model : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			Mesh->Create();
			
			if (mSaveMeshesPaths)
				mHashedMeshesPaths[hashedname] = Path;

			if (mSaveMaterialsPaths && desc.LoadMaterial)
				mHashedMaterialsPaths[hashedname] = Path;


			return { Mesh , Material, anim };
		}

					
		Assets::Image* AssetManager::TextureCube_Load(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			auto doesitexist = DoesImageExist(hashedname);
			if (doesitexist != nullptr)
			{
				return doesitexist;
			}

			Assets::Image result;
			Assets::ImageData imagedata = mImageImporter(Path.mRealPath, Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager : '{0}'] Failed To Load Texture2D (For CubeMap): '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
				return nullptr;
			}
			result.mData = imagedata;

			mImportedImages[hashedname] = result;
			
			NUCLEAR_INFO("[AssetManager : '{0}'] Imported Texture2D (for CubeMap) : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			return &mImportedImages[hashedname];
		}
		Assets::Image* AssetManager::DoesImageExist(Uint32 hashedname)
		{
			//Check if Texture has been loaded before
			auto it = mImportedImages.find(hashedname);
			if (it != mImportedImages.end())
			{
				return &it->second;
			}
			return nullptr;
		}
		std::array<Assets::Image*, 6> AssetManager::LoadTextureCubeFromFile(const std::array<Core::Path, 6>& Paths, const Importers::ImageLoadingDesc& desc)
		{
			Importers::ImageLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Assets::Image*, 6> result = { TextureCube_Load(Paths.at(0), Desc),
				TextureCube_Load(Paths.at(1), Desc), 
				TextureCube_Load(Paths.at(2), Desc), 
				TextureCube_Load(Paths.at(3), Desc),
				TextureCube_Load(Paths.at(4), Desc),
				TextureCube_Load(Paths.at(5), Desc) };

			return result;
		}

		void AssetManager::SaveScene(ECS::Scene* scene)
		{
			std::stringstream storage;

			{
				// output finishes flushing its contents when it goes out of scope
				cereal::JSONOutputArchive output{ storage };
				entt::snapshot{ scene->GetRegistry()}.entities(output).component<
					Components::EntityInfoComponent/*,
					Components::CameraComponent,
Components::MeshComponent,
Components::ColliderComponent,
Components::RigidBodyComponent,
Components::DirLightComponent,
Components::PointLightComponent,
Components::SpotLightComponent,
Components::AnimatorComponent,
Components::ScriptComponent*/

				>(output);
			}
		}

		//Assets::Script& AssetManager::ImportScript(const Core::Path& Path)
		//{
		//	Assets::Script result;

		//	//result.ScriptCode = Core::FileSystem::LoadFileToString(Path);

		//	return result;
		//}
	}
}