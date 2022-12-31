#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/Image.h>
#include <Assets/Importers/FreeImageImporter.h>
#include <Core/Path.h>
#include <Assets/Importer.h>

#include <Threading/Task.h>
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Assets
	{
		class Image;
		struct AssetInfo
		{
			Core::Path mPath;
			Uint32 mHashedPath;
			std::string mLibraryName;
			bool mLog = true;
		};
		namespace Importers
		{

			class CreateImageTask : public Threading::MainThreadTask
			{
			public:
				CreateImageTask(Image* img, const AssetInfo& info, const Diligent::TextureDesc& texdesc, const Diligent::TextureData& texdata)
					: pImage(img), mDesc(texdesc), mData(texdata), mInfo(info)
				{

				}

				bool OnRunning() override
				{
					bool result = pImage->Create(mDesc, mData);
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
					Importer::FinishImportingAsset(pImage, mInfo.mPath, mInfo.mHashedPath, mInfo.mLibraryName, mInfo.mLog);
					return result;
				}

				void OnEnd() override
				{
					delete this;
				}
				Image* pImage;
				TextureDesc mDesc;
				TextureData mData;

				AssetInfo mInfo;
			};

			class NEAPI ImportImageTask : public Threading::Task
			{
			public:
				ImportImageTask(Assets::Image* result, const AssetInfo& info, const Assets::ImageImportingDesc& desc = ImageImportingDesc())
					: pResult(result), mInfo(info)
				{
					pResult = result;
					mDesc = desc;
				}
				~ImportImageTask()
				{

				}
				bool OnRunning() override
				{
					Assets::ImageData data;
					bool result = Importers::FreeImageImporter::GetInstance().Load(mInfo.mPath.GetRealPath(), &data, mDesc);

					if (result)
					{
						pResult->SetState(IAsset::State::Loaded);
						Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateImageTask(pResult, mInfo, data, mDesc));
					}
					else
						NUCLEAR_ERROR("[Importer] Failed To Import Image: '{0}' Hash: '{1}'", mInfo.mPath.GetInputPath(), Utilities::int_to_hex<Uint32>(mInfo.mHashedPath));

					return result;
				}

				void OnEnd() override
				{
					delete this;
				}
			protected:
				Assets::Image* pResult;
				Assets::ImageImportingDesc mDesc;
				AssetInfo mInfo;
			};

			class NEAPI LoadImageTask : public Threading::Task
			{
			public:
				LoadImageTask(Assets::Image* result, const AssetInfo& info, const Assets::ImageLoadingDesc& desc = ImageLoadingDesc())
					: pResult(result), mInfo(info)
				{
					pResult = result;
					mDesc = desc;
				}
				~LoadImageTask()
				{

				}
				bool OnRunning() override
				{
					Assets::ImageData data;
					bool result = Importers::FreeImageImporter::GetInstance().Load(&data, mDesc);

					if (result)
					{
						pResult->SetState(IAsset::State::Loaded);
						Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new CreateImageTask(pResult, mInfo, data, mDesc));
					}
					else
						NUCLEAR_ERROR("[Importer] Failed To Import Image: '{0}' Hash: '{1}'", mInfo.mPath.GetInputPath(), Utilities::int_to_hex<Uint32>(mInfo.mHashedPath));

					return result;
				}

				void OnEnd() override
				{
					delete this;
				}
			protected:
				Assets::Image* pResult;
				Assets::ImageLoadingDesc mDesc;
				AssetInfo mInfo;
			};

		}
	}
}