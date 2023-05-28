#pragma once
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingModule.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Loader.h>
#include <Assets/AssetLibrary.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>
#include <Utilities/Logger.h>

#include <Serialization/SerializationModule.h>

namespace Nuclear
{
	namespace Assets
	{
		class Material;

		class MaterialLoadTask : public Threading::Task
		{
		public:
			MaterialLoadTask(Assets::Material* result, const Core::Path& path, const Assets::AssetMetadata& metadata)
				: pResult(result), mPath(path), mAssetmeta(metadata)
			{
			}
			~MaterialLoadTask()
			{

			}
			bool OnRunning() override
			{
				//load
				Serialization::BinaryBuffer buffer;
				Platform::FileSystem::Get().LoadBinaryBuffer(buffer, mPath.GetPathNoExt() + ".NEMaterial");
				zpp::bits::in in(buffer);

				auto err = in(*pResult);
				
				//delete from queued assets
				auto& vec = Loader::Get().GetQueuedAssets();
				for (Uint32 i = 0; i < vec.size(); i++)
				{
					if (vec.at(i)->GetUUID() == pResult->GetUUID())
					{
						vec.erase(vec.begin() + i);
						break;
					}
				}

				if (failure(err))
				{
					NUCLEAR_TRACE("[Assets] Failed to de-serialize material '{0}'", mPath.GetInputPath());
					pResult->SetState(IAsset::State::Unknown);
					return false;
				}

				pResult->SetState(IAsset::State::Created);

				NUCLEAR_TRACE("[Assets] Loaded: {0} ", mPath.GetInputPath());

				return true;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::Material* pResult;
			Assets::AssetMetadata mAssetmeta;
			Core::Path mPath;
		};

	}
}