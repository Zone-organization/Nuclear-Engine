#pragma once
#include <Assets/Tasks/TextureCreateTask.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Importers/TextureImporter.h>
#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>

#include <Serialization/SerializationModule.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		
		class TextureImportTask : public Threading::Task
		{
		public:
			TextureImportTask(Assets::Texture* result, const Core::Path& path, const Assets::TextureImportingDesc& desc)
				: pResult(result), mPath(path), mImportingDesc(desc)
			{
			}
			~TextureImportTask()
			{
				
			}
			bool OnRunning() override
			{
				//Import
				TextureData* pResultData = new TextureData;
				Assets::TextureDesc desc;
				bool result = false;

				if (!mImportingDesc.mLoadFromMemory)
				{
					auto extension = Importers::TextureImporter::Get().GetImageExtension(mPath.GetRealPath());
					auto importedfile = Platform::FileSystem::Get().LoadFile(mPath.GetRealPath());
					mImportingDesc.mMemData = importedfile.mDataBuf.data();
					mImportingDesc.mMemSize = importedfile.mDataBuf.size();
					result = Importers::TextureImporter::Get().Import(pResultData, extension, mImportingDesc);
				}
				else
				{
					result = Importers::TextureImporter::Get().Import(pResultData, IMAGE_EXTENSION_UNKNOWN, mImportingDesc);
					if (mImportingDesc.mEngineAllocMem)
					{
						free(mImportingDesc.mMemData);
					}
				}

				if (result)
				{
					pResult->SetState(IAsset::State::Loaded);

					if (!mImportingDesc.mCommonOptions.mLoadOnly)
					{
						std::string exportpath = mImportingDesc.mCommonOptions.mExportPath.GetRealPath();
						if (!mImportingDesc.mCommonOptions.mExportPath.isValid())
						{
							exportpath = AssetLibrary::Get().GetPath() + "Textures/";
						}
						Platform::FileSystem::Get().CreateDir(exportpath);

						std::string exportedimagename = pResult->GetName() + ".dds"; ///<TODO extension...

						Importers::TextureImporter::Get().Export(exportpath + exportedimagename, pResultData, mImportingDesc.mExportExtension);

						AssetMetadata assetmetadata = Assets::AssetManager::Get().CreateMetadata(pResult);

						auto imageloadingdesc = static_cast<Assets::TextureLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::TextureLoadingDesc);

						imageloadingdesc->mExtension = mImportingDesc.mExportExtension;
						imageloadingdesc->mAsyncLoading = mImportingDesc.mCommonOptions.mAsyncImport;

						//Export Meta
						Serialization::SerializationModule::Get().Serialize(assetmetadata, exportpath + exportedimagename + ".NEMeta");

						delete assetmetadata.pLoadingDesc;
					}
					//Create image task
					Threading::ThreadingModule::Get().AddMainThreadTask(new TextureCreateTask(pResult, pResultData, mPath, desc, IMPORTER_FACTORY_TYPE));
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
			Assets::TextureImportingDesc mImportingDesc;
			Core::Path mPath;
		};

	}
}