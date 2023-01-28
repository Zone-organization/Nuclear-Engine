#include <Assets\Loader.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Tasks/TextureLoadTask.h>
#include <Assets/Tasks/MaterialLoadTask.h>
#include <Assets/Tasks/ShaderLoadTask.h>
#include <Fallbacks/FallbacksEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		Loader::Loader()
		{

		}
		Loader& Loader::GetInstance()
		{
			static Loader instance;
			return instance;
		}
		Texture* Loader::LoadTexture(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::GetInstance().mImportedTextures.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			if (result->GetTextureView() == nullptr)
			{
				result->SetTextureView(Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage()->GetTextureView());
			}

			mQueuedAssets.push_back(result);

			Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new TextureLoadTask(result, Path, metadata));

			return result;
		}
		Material* Loader::LoadMaterial(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::GetInstance().mImportedMaterials.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			mQueuedAssets.push_back(result);

			Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new MaterialLoadTask(result, Path, metadata));

			return result;
		}
		Shader* Loader::LoadShader(const Core::Path& Path, const Assets::AssetMetadata& metadata)
		{
			//Add to queue			
			auto result = AssetLibrary::GetInstance().mImportedShaders.GetOrAddAsset(metadata.mUUID);
			result->SetName(metadata.mName);
			result->SetState(IAsset::State::Queued);

			mQueuedAssets.push_back(result);

			Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new ShaderLoadTask(result, Path, metadata));

			return result;
		}
		std::vector<IAsset*>& Loader::GetQueuedAssets()
		{
			return mQueuedAssets;
		}
	}
}