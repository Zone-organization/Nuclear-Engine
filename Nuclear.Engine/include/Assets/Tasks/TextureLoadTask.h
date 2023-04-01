#pragma once
#include <Assets/Tasks/TextureCreateTask.h>
#include <Assets/Importers/TextureImporter.h>
#include <Core/Path.h>
#include <Assets/Loader.h>
#include <Assets/AssetLibrary.h>

#include <Assets/LoadingDescs.h>

#include <Platform/FileSystem.h>

#include <Serialization/SerializationModule.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
			
		class TextureLoadTask : public Threading::Task
		{
		public:
			TextureLoadTask(Assets::Texture* result, const Core::Path& path, const Assets::AssetMetadata& metadata)
				: pResult(result), mPath(path), mAssetmeta(metadata)
			{
			}
			~TextureLoadTask()
			{

			}
			bool OnRunning() override
			{
				Assets::TextureLoadingDesc mLoadingDesc = *static_cast<Assets::TextureLoadingDesc*>(mAssetmeta.pLoadingDesc);
				delete mAssetmeta.pLoadingDesc;

				//Load image
				TextureData* pResultData = new TextureData;
				pResultData->mData = Platform::FileSystem::GetInstance().LoadFile(mPath.GetPathNoExt());
				Assets::TextureDesc desc;

				bool result = Importers::TextureImporter::GetInstance().Load(mLoadingDesc, pResultData);

				if (result)
				{
					pResult->SetState(IAsset::State::Loaded);

					//Create image task
					Threading::ThreadingModule::GetInstance().AddMainThreadTask(new TextureCreateTask(pResult, pResultData, mPath, desc, LOADER_FACTORY_TYPE));
				}
				else
				{
					delete pResultData;
					NUCLEAR_ERROR("[Assets] Failed To Load Texture: '{0}'", mPath.GetInputPath());
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
			Core::Path mPath;
		};
	}
}