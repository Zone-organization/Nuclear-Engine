#include <Physics3D\PhysicalScene.h>
#include <PhysX\PhysXincluder.h>

#if !defined(PHYSX_NOT_INCLUDED) && defined(NE_USE_PHYSXENGINE)
#include <Physics3D\Physics3DEngine.h>
#include <PhysX\Include\PxPhysicsAPI.h>
#include "PhysXCommon.h"
using namespace physx;

namespace NuclearEngine
{
	namespace Physics3D
	{
		bool PhysicalScene::Create(PhysicalScene* scene, const PhysicalSceneDesc& desc)
		{
			//Create the scene
			PxSceneDesc sceneDesc(Physics3DEngine::GetContext()->getTolerancesScale());

			sceneDesc.gravity = Convert(desc.gravity);


			if (!sceneDesc.cpuDispatcher) 
			{
				PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
				if (!mCpuDispatcher)
				{
					Log.Error("[PhysicalScene] Failed to create PhysicalScene: PxDefaultCpuDispatcherCreate Failed!\n");
					return false;
				}
				sceneDesc.cpuDispatcher = mCpuDispatcher;
			}
			if (!sceneDesc.filterShader)
			{
				//sceneDesc.filterShader = gDefaultFilterShader;
			}
			scene->scene = Physics3DEngine::GetContext()->createScene(sceneDesc);
			if (!scene->scene)
			{
				Log.Error("[PhysicalScene] PhysX failed to create PhysicalScene!\n");
				return false;
			}
			scene->scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
			scene->scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			return true;
		}
		void PhysicalScene::Delete(PhysicalScene* scene)
		{

		}

		physx::PxScene * PhysicalScene::GetBase()
		{
			return scene;
		}
	}
}
#else
namespace NuclearEngine
{
	namespace Physics3D
	{

		bool PhysicalScene::Create(PhysicalScene* scene)
		{

			return false;
		}
		void PhysicalScene::Delete(PhysicalScene* scene)
		{

		}
		physx::PxScene * PhysicalScene::GetBase()
		{
			return nullptr;
		}
	}
}
#endif