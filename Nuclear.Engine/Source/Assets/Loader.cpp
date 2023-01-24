#include <Assets\Loader.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Tasks/TextureLoadTask.h>
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
			result->SetTextureView(Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage()->GetTextureView());

			mQueuedAssets.push_back(result);

			Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new TextureLoadTask(result, Path, metadata));

			return result;
		}
		std::vector<IAsset*>& Loader::GetQueuedAssets()
		{
			return mQueuedAssets;
		}
	}
}