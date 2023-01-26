#pragma once
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Loader.h>
#include <Assets/AssetLibrary.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>
#include <Utilities/Logger.h>

#include <Serialization/SerializationEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Shader;

		class ShaderLoadTask : public Threading::Task
		{
		public:
			ShaderLoadTask(Assets::Shader* result, const Core::Path& path, const Assets::AssetMetadata& metadata)
				: pResult(result), mPath(path), mAssetmeta(metadata)
			{
			}
			~ShaderLoadTask()
			{

			}
			bool OnRunning() override
			{
				//load
				Serialization::BinaryBuffer buffer;
				Platform::FileSystem::GetInstance().LoadBinaryBuffer(buffer, mPath.GetPathNoExt() + ".NEShader");
				zpp::bits::in in(buffer);
				in(*pResult);

				//delete from queued assets
				auto& vec = Loader::GetInstance().GetQueuedAssets();
				for (Uint32 i = 0; i < vec.size(); i++)
				{
					if (vec.at(i)->GetUUID() == pResult->GetUUID())
					{
						vec.erase(vec.begin() + i);
						break;
					}
				}

				pResult->SetState(IAsset::State::Loaded);

				NUCLEAR_INFO("[Assets] Loaded: {0} ", mPath.GetInputPath());

				return true;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Assets::Shader* pResult;
			Assets::AssetMetadata mAssetmeta;
			Core::Path mPath;
		};

	}
}