#pragma once
#include <Assets/Tasks/TextureCreateTask.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Importers/TextureImporter.h>
#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		
		class TextureImportTask : public Threading::Task
		{
		public:
			TextureImportTask(Assets::Texture* result, const Core::Path& path, const Assets::TextureImportingDesc& desc)
				: pResult(result), mPath(path), pResultData(nullptr), mImportingDesc(desc)
			{
			}
			~TextureImportTask()
			{
				
			}
			bool OnRunning() override
			{
				//Import
				pResultData = new TextureData;
				Assets::TextureDesc desc;
				bool result = false;

				if (!mImportingDesc.mLoadFromMemory)
				{
					auto extension = Importers::TextureImporter::GetInstance().GetImageExtension(mPath.GetRealPath());
					auto importedfile = Platform::FileSystem::GetInstance().LoadFile(mPath.GetRealPath());
					mImportingDesc.mMemData = importedfile.mDataBuf.data();
					mImportingDesc.mMemSize = importedfile.mDataBuf.size();
					result = Importers::TextureImporter::GetInstance().Import(pResultData, extension, mImportingDesc);
				}
				else
				{
					assert(false);
				}
				if (result)
				{
					pResult->SetState(IAsset::State::Loaded);

					if (!mImportingDesc.mCommonOptions.mLoadOnly)
					{
						std::string exportpath = mImportingDesc.mCommonOptions.mExportPath.GetRealPath();
						if (!mImportingDesc.mCommonOptions.mExportPath.isValid())
						{
							exportpath = AssetLibrary::GetInstance().GetPath() + "Textures/";
						}

						std::string exportedimagename = pResult->GetName() + ".dds"; ///<TODO extension...

						Importers::TextureImporter::GetInstance().Export(exportpath + exportedimagename, pResultData, mImportingDesc.mExportExtension);

						AssetMetadata assetmetadata = Assets::AssetManager::GetInstance().CreateMetadata(pResult);

						auto imageloadingdesc = static_cast<Assets::TextureLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::TextureLoadingDesc);

						imageloadingdesc->mExtension = mImportingDesc.mExportExtension;
						imageloadingdesc->mAsyncLoading = mImportingDesc.mCommonOptions.mAsyncImport;

						//Export Meta
						Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, exportpath + exportedimagename + ".NEMeta");

						delete assetmetadata.pLoadingDesc;
					}
					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new TextureCreateTask(pResult, pResultData, mPath, desc, IMPORTER_FACTORY_TYPE));
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
			Assets::Texture* pResult;
			TextureData* pResultData;
			Assets::TextureImportingDesc mImportingDesc;
			Core::Path mPath;
		};

	}
}