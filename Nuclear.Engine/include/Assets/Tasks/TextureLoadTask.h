#pragma once
#include <Assets/Tasks/TextureCreateTask.h>
#include <Assets/Importers/TextureImporter.h>
#include <Core/Path.h>
#include <Assets/Loader.h>
#include <Assets/AssetLibrary.h>

#include <Assets/LoadingDescs.h>

#include <Platform/FileSystem.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
			
		class TextureLoadTask : public Threading::Task
		{
		public:
			TextureLoadTask(Assets::Texture* result, const Core::Path& path, const Assets::AssetMetadata& metadata)
				: pResult(result), mPath(path), pResultData(nullptr), mAssetmeta(metadata)
			{
			}
			~TextureLoadTask()
			{

			}
			bool OnRunning() override
			{
				Assets::TextureLoadingDesc mLoadingDesc = *static_cast<Assets::TextureLoadingDesc*>(mAssetmeta.pLoadingDesc);
				delete mAssetmeta.pLoadingDesc;
				mLoadingDesc.mData = Platform::FileSystem::GetInstance().LoadFile(mPath.GetPathNoExt() + ".dds");

				//Load image
				pResultData = new TextureData;
				Assets::TextureDesc desc;

				bool result = Importers::TextureImporter::GetInstance().Load(mLoadingDesc, pResultData);

				if (result)
				{
					std::string exportpath = AssetLibrary::GetInstance().GetPath() + "Textures/";

					pResult->SetState(IAsset::State::Loaded);

					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new TextureCreateTask(pResult, pResultData, mPath, desc, &Loader::GetInstance().GetQueuedAssets()));
				}
				else
				{
					delete pResultData;
					NUCLEAR_ERROR("[Importer] Failed To Import Texture: '{0}'", mPath.GetInputPath());
				}
				return result;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::AssetMetadata mAssetmeta;
			Assets::Texture* pResult;
			TextureData* pResultData;
			Core::Path mPath;
		};
	}
}