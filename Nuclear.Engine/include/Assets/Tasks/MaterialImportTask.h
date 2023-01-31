#pragma once
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Assets/ImportingDescs.h>
#include <filesystem>
#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>
#include <Utilities/Logger.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Material;

		class MaterialImportTask : public Threading::Task
		{
		public:
			MaterialImportTask(Assets::Material* result, const Core::Path& path, const Assets::MaterialImportingDesc& desc)
				: pResult(result), mPath(path), mImportingDesc(desc)
			{
			}
			~MaterialImportTask()
			{

			}
			bool OnRunning() override
			{
				namespace fs = std::filesystem;

				std::string newpath = AssetLibrary::GetInstance().GetPath() + "Materials/" + mPath.GetFilename(true);
				std::error_code ec;
				fs::copy_file(mPath.GetRealPath(), newpath + ".NEMaterial", ec);

				if (ec)
				{
					NUCLEAR_ERROR("[Importer] Importing Material '{0}' Failed : '{1}'", mPath.GetInputPath(), ec.message());
					return false;
				}

				//load
				Serialization::BinaryBuffer buffer;
				Platform::FileSystem::GetInstance().LoadBinaryBuffer(buffer, newpath);
				zpp::bits::in in(buffer);
				in(*pResult);

				//delete from queued assets
				auto& vec = Importer::GetInstance().GetQueuedAssets();
				for (Uint32 i = 0; i < vec.size(); i++)
				{
					if (vec.at(i)->GetUUID() == pResult->GetUUID())
					{
						vec.erase(vec.begin() + i);
						break;
					}
				}
			
				pResult->SetState(IAsset::State::Created);

				//export meta
				AssetMetadata assetmetadata = Assets::AssetManager::GetInstance().CreateMetadata(pResult);

				auto matloadingdesc = static_cast<Assets::MaterialLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::MaterialLoadingDesc);

				Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, newpath + ".NEMaterial" + ".NEMeta");
				delete assetmetadata.pLoadingDesc;

				NUCLEAR_INFO("[Assets] Imported: {0} ", mPath.GetInputPath());

				return true;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::Material* pResult;
			Assets::MaterialImportingDesc mImportingDesc;
			Core::Path mPath;
		};

	}
}