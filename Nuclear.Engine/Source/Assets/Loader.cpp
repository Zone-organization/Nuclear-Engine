#include <Assets\Loader.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Tasks/TextureLoadTask.h>
#include <Assets/Tasks/MaterialLoadTask.h>
#include <Assets/Tasks/ShaderLoadTask.h>
#include <Assets/Tasks/MeshLoadTask.h>
#include <Fallbacks/FallbacksModule.h>

namespace Nuclear
{
	namespace Assets
	{
		Loader::Loader()
		{

		}
		Loader& Loader::Get()
		{
			static Loader instance;
			return instance;
		}
		Texture* Loader::LoadTexture(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::Get().mImportedTextures.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			if (result->GetTextureView() == nullptr)
			{
				result->SetTextureView(Fallbacks::FallbacksModule::Get().GetDefaultBlackImage()->GetTextureView());
			}

			mQueuedAssets.push_back(result);

			Threading::ThreadingModule::Get().GetThreadPool().AddTask(new TextureLoadTask(result, Path, metadata));

			return result;
		}
		Material* Loader::LoadMaterial(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::Get().mImportedMaterials.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			mQueuedAssets.push_back(result);

			Threading::ThreadingModule::Get().GetThreadPool().AddTask(new MaterialLoadTask(result, Path, metadata));

			return result;
		}
		Shader* Loader::LoadShader(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::Get().mImportedShaders.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			mQueuedAssets.push_back(result);

			Threading::ThreadingModule::Get().GetThreadPool().AddTask(new ShaderLoadTask(result, Path, metadata));

			return result;
		}
		Mesh* Loader::LoadMesh(const Core::Path& Path, bool auto_load_external_material, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			Mesh* mesh = AssetLibrary::Get().mImportedMeshes.GetOrAddAsset(metadata.mUUID);
			mesh->SetName(metadata.mName);
			mesh->SetState(IAsset::State::Queued);


			Material* material = nullptr;
			Animations* animations = nullptr;

			Assets::MeshLoadingDesc mLoadingDesc = *static_cast<Assets::MeshLoadingDesc*>(metadata.pLoadingDesc);

			if (mLoadingDesc.mMaterialUUID.isValid())
			{
				if (mLoadingDesc.mExternalMaterial)
				{
					if (auto_load_external_material)
					{
						AssetMetadata material_meta;
						if (!Serialization::SerializationModule::Get().Deserialize(material_meta, Path.GetPathNoExt() + ".NEMaterial.NEMeta"))
						{
							NUCLEAR_ERROR("[Loader] Loading Mesh's External Material with no metadata! : '{0}'", Path.GetInputPath());
							return nullptr;
						}
						material = LoadMaterial(Path.GetPathNoExt() + ".NEMaterial", material_meta);
					}
				}
				else
				{
					material = AssetLibrary::Get().mImportedMaterials.GetOrAddAsset(mLoadingDesc.mMaterialUUID);
					mQueuedAssets.push_back(material);
				}
			}
			if (mLoadingDesc.mAnimationsUUID.isValid())
			{
				animations = AssetLibrary::Get().mImportedAnimations.GetOrAddAsset(mLoadingDesc.mAnimationsUUID);
				mQueuedAssets.push_back(animations);
			}

			mesh->pImportedAnimations = animations;
			mesh->pImportedMaterial = material;

			mQueuedAssets.push_back(mesh);

			Threading::ThreadingModule::Get().GetThreadPool().AddTask(new MeshLoadTask({ metadata.mName, mesh, material, animations }, Path, metadata));

			return mesh;
		}
		std::vector<IAsset*>& Loader::GetQueuedAssets()
		{
			return mQueuedAssets;
		}
	}
}