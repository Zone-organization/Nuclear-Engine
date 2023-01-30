#pragma once
#include <Core/Path.h>
#include <Assets/Importer.h>
#include <Assets/Loader.h>
#include <Assets/Mesh.h>

#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Utilities/Logger.h>

#include <Graphics\GraphicsEngine.h>

#define IMPORTER_FACTORY_TYPE 1
#define LOADER_FACTORY_TYPE 2

namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class MeshCreateTask : public Threading::MainThreadTask
		{
		public:
			//type int : 
			//1: importer
			//2: loader
			//other
			MeshCreateTask(Mesh* mesh,const Core::Path& path, int factory_type)
				: pMesh(mesh), mPath(path), mFactoryType(factory_type)
			{
			}

			bool OnRunning() override
			{
				bool result = Graphics::GraphicsEngine::GetInstance().CreateMesh(pMesh);

				if (mFactoryType == 1)
				{
					auto& vec = Importer::GetInstance().GetQueuedAssets();
					for (Uint32 i = 0; i < vec.size(); i++)
					{
						if (vec.at(i)->GetUUID() == pMesh->GetUUID())
						{
							vec.erase(vec.begin() + i);
							break;
						}
					}
				}
				else if (mFactoryType == 2)
				{
					auto& vec = Loader::GetInstance().GetQueuedAssets();
					for (Uint32 i = 0; i < vec.size(); i++)
					{
						if (vec.at(i)->GetUUID() == pMesh->GetUUID())
						{
							vec.erase(vec.begin() + i);
							break;
						}
					}
				}


				if (!result)
				{
					NUCLEAR_ERROR("[MeshCreateTask] Failed To Create Mesh: '{0}'", mPath.GetInputPath());
					return false;
				}

				if (mFactoryType == 1)
				{
					NUCLEAR_INFO("[Assets] Imported: {0} ", mPath.GetInputPath());
				}
				else if (mFactoryType == 2)
				{
					NUCLEAR_INFO("[Assets] Loaded: {0} ", mPath.GetInputPath());
				}
				else
				{
					NUCLEAR_INFO("[MeshCreateTask] Created: {0} ", mPath.GetInputPath());
				}

				return result;
			}

			void OnEnd() override
			{
				delete this;
			}

		protected:
			Mesh* pMesh;
			Core::Path mPath;
			int mFactoryType;
		};
	}
}