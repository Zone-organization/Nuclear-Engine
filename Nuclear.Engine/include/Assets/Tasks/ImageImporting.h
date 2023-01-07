#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/Image.h>
#include <Assets/Importers/ImageImporter.h>
#include <Core/Path.h>
#include <Assets/Importer.h>

#include <Threading/Task.h>
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Utilities/Logger.h>

#include <Graphics\GraphicsEngine.h>
#include <Assets/LoadingDescs.h>

#include <Parsers/INIParser.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Image;
		struct AssetInfo
		{
			Core::Path mPath;
			Uint32 mHashedPath;
			bool mLog = true;
		};

		class CreateImageTask : public Threading::MainThreadTask
		{
		public:
			CreateImageTask(Image* img, ImageData* data, const AssetInfo& info, const Assets::ImageDesc& desc)
				: pImage(img), pImageData(data), mInfo(info), mDesc(desc)
			{
			}

			bool OnRunning() override
			{
				bool result = Graphics::GraphicsEngine::GetInstance().CreateImage(pImage, pImageData);
				auto& vec = Importer::GetInstance().GetQueuedAssets();

				for (Uint32 i = 0; i < vec.size(); i++)
				{
					if (vec[i]->GetUUID() == pImage->GetUUID())
					{
						vec.erase(vec.begin() + i);
						break;
					}
				}

				if (!result)
				{
					NUCLEAR_ERROR("[Importer] Failed To Create Image: '{0}' Hash: '{1}'", mInfo.mPath.GetInputPath(), Utilities::int_to_hex<Uint32>(mInfo.mHashedPath));
					return false;
				}
				Importer::FinishImportingAsset(pImage, mInfo.mPath, mInfo.mHashedPath, mInfo.mLog);
				return result;
			}

			void OnEnd() override
			{
				delete pImageData;
				delete this;
			}
			Assets::ImageDesc mDesc;
			Image* pImage;
			ImageData* pImageData;
			AssetInfo mInfo;
		};
		
		class ImageImportTask : public Threading::Task
		{
		public:
			ImageImportTask(Assets::Image* result, const AssetInfo& info, const Assets::ImageImportingDesc& desc)
				: pResult(result), mInfo(info), pResultData(nullptr), mImportingDesc(desc)
			{
			}
			~ImageImportTask()
			{

			}
			bool OnRunning() override
			{
				//Import
				pResultData = new ImageData;
				Assets::ImageDesc desc;
				bool result = false;

				if (!mImportingDesc.mLoadFromMemory)
				{
					auto extension = Importers::ImageImporter::GetInstance().GetImageExtension(mInfo.mPath.GetRealPath());
					auto importedfile = Platform::FileSystem::GetInstance().LoadFile(mInfo.mPath.GetRealPath());
					mImportingDesc.mMemData = importedfile.mDataBuf.data();
					mImportingDesc.mMemSize = importedfile.mDataBuf.size();
					result = Importers::ImageImporter::GetInstance().Import(pResultData, extension, mImportingDesc);
				}
				else
				{
					assert(false);
				}
				if (result)
				{
					pResult->SetState(IAsset::State::Loaded);

					if (!mImportingDesc.mLoadOnly)
					{
						std::string exportpath = AssetLibrary::GetInstance().mPath.GetRealPath() + "/Textures/";
						std::string exportedimagename = mInfo.mPath.GetFilename(true) + ".dds"; ///<TODO extension...

						Importers::ImageImporter::GetInstance().Export(exportpath + exportedimagename, pResultData, mImportingDesc.mExportExtension);

						AssetMetadata assetmetadata = Assets::AssetManager::GetInstance().CreateMetadata(pResult);

						auto imageloadingdesc = static_cast<Assets::ImageLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::ImageLoadingDesc);

						imageloadingdesc->mExtension = mImportingDesc.mExportExtension;
						imageloadingdesc->mAsyncLoading = mImportingDesc.mAsyncImporting;

						//Export Meta
						Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, exportpath + mInfo.mPath.GetFilename(true) + ".NEAsset");
					}
					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateImageTask(pResult, pResultData, mInfo, desc));
				}
				else
				{
					delete pResultData;
					NUCLEAR_ERROR("[Importer] Failed To Import Image: '{0}' Hash: '{1}'", mInfo.mPath.GetInputPath(), Utilities::int_to_hex<Uint32>(mInfo.mHashedPath));
				}
				return result;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::Image* pResult;
			ImageData* pResultData;
			Assets::ImageImportingDesc mImportingDesc;
			AssetInfo mInfo;
		};

		class ImageLoadTask : public Threading::Task
		{
		public:
			ImageLoadTask(Assets::Image* result, const AssetInfo& info)
				: pResult(result), mInfo(info), pResultData(nullptr)
			{
			}
			~ImageLoadTask()
			{

			}
			bool OnRunning() override
			{		
				Assets::AssetMetadata assetmeta;

				Serialization::SerializationEngine::GetInstance().Deserialize(assetmeta, mInfo.mPath);
				Assets::ImageLoadingDesc mLoadingDesc = *static_cast<Assets::ImageLoadingDesc*>(assetmeta.pLoadingDesc);
				delete assetmeta.pLoadingDesc;
				mLoadingDesc.mData = Platform::FileSystem::GetInstance().LoadFile(mInfo.mPath.GetPathNoExt() + ".dds");

				//Load image
				pResultData = new ImageData;
				Assets::ImageDesc desc;

				bool result = Importers::ImageImporter::GetInstance().Load(mLoadingDesc, pResultData);

				if (result)
				{
					std::string exportpath = AssetLibrary::GetInstance().mPath.GetRealPath() + "/Textures/";

					pResult->SetState(IAsset::State::Loaded);

					Importers::ImageImporter::GetInstance().Export(exportpath + mInfo.mPath.GetFilename(true), pResultData, mLoadingDesc.mExtension);

					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateImageTask(pResult, pResultData, mInfo, desc));
				}
				else
				{
					delete pResultData;
					NUCLEAR_ERROR("[Importer] Failed To Import Image: '{0}' Hash: '{1}'", mInfo.mPath.GetInputPath(), Utilities::int_to_hex<Uint32>(mInfo.mHashedPath));
				}
				return result;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::Image* pResult;
			ImageData* pResultData;
			AssetInfo mInfo;
		};
	}
}