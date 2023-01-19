#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/Texture.h>
#include <Assets/Importers/TextureImporter.h>
#include <Core/Path.h>
#include <Assets/Importer.h>

#include <Threading/Task.h>
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Utilities/Logger.h>

#include <Graphics\GraphicsEngine.h>
#include <Assets/LoadingDescs.h>
#include <Assets/AssetManager.h>

#include <Parsers/INIParser.h>
#include <Platform/FileSystem.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;

		class CreateTextureTask : public Threading::MainThreadTask
		{
		public:
			CreateTextureTask(Texture* img, TextureData* data, const Core::Path& path, const Assets::TextureDesc& desc)
				: pTexture(img), pImageData(data), mPath(path), mDesc(desc)
			{
			}

			bool OnRunning() override
			{
				bool result = Graphics::GraphicsEngine::GetInstance().CreateImage(pTexture, pImageData);
				auto& vec = Importer::GetInstance().GetQueuedAssets();

				for (Uint32 i = 0; i < vec.size(); i++)
				{
					if (vec[i]->GetUUID() == pTexture->GetUUID())
					{
						vec.erase(vec.begin() + i);
						break;
					}
				}

				if (!result)
				{
					NUCLEAR_ERROR("[Importer] Failed To Create Texture: '{0}'", mPath.GetInputPath());
					return false;
				}

				NUCLEAR_INFO("[Assets] Imported: {0} ", mPath.GetInputPath());
				return result;
			}

			void OnEnd() override
			{
				delete pImageData;
				delete this;
			}
			Assets::TextureDesc mDesc;
			Texture* pTexture;
			TextureData* pImageData;
			Core::Path mPath;
		};
		
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

						auto imageloadingdesc = static_cast<Assets::ImageLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::ImageLoadingDesc);

						imageloadingdesc->mExtension = mImportingDesc.mExportExtension;
						imageloadingdesc->mAsyncLoading = mImportingDesc.mCommonOptions.mAsyncImport;

						//Export Meta
						Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, exportpath + exportedimagename + ".NEAsset");
					}
					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateTextureTask(pResult, pResultData, mPath, desc));
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

		class TextureLoadTask : public Threading::Task
		{
		public:
			TextureLoadTask(Assets::Texture* result, const Core::Path& path)
				: pResult(result), mPath(path), pResultData(nullptr)
			{
			}
			~TextureLoadTask()
			{

			}
			bool OnRunning() override
			{		
				Assets::AssetMetadata assetmeta;

				Serialization::SerializationEngine::GetInstance().Deserialize(assetmeta, mPath);
				Assets::ImageLoadingDesc mLoadingDesc = *static_cast<Assets::ImageLoadingDesc*>(assetmeta.pLoadingDesc);
				delete assetmeta.pLoadingDesc;
				mLoadingDesc.mData = Platform::FileSystem::GetInstance().LoadFile(mPath.GetPathNoExt() + ".dds");

				//Load image
				pResultData = new TextureData;
				Assets::TextureDesc desc;

				bool result = Importers::TextureImporter::GetInstance().Load(mLoadingDesc, pResultData);

				if (result)
				{
					std::string exportpath = AssetLibrary::GetInstance().GetPath() + "Textures/";

					pResult->SetState(IAsset::State::Loaded);

					Importers::TextureImporter::GetInstance().Export(exportpath + mPath.GetFilename(true), pResultData, mLoadingDesc.mExtension);

					//Create image task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateTextureTask(pResult, pResultData, mPath, desc));
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
			Core::Path mPath;
		};
	}
}