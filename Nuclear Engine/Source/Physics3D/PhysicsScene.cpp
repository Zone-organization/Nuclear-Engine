#include <Physics3D\PhysicsScene.h>
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
		bool PhysicsScene::Create(PhysicsScene* scene, const PhysicsSceneDesc& desc)
		{
			//Create the scene
			PxSceneDesc sceneDesc(Physics3DEngine::GetContext()->getTolerancesScale());

			sceneDesc.gravity = Convert(desc.gravity);


			if (!sceneDesc.cpuDispatcher) 
			{
				PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
				if (!mCpuDispatcher)
				{
					Log.Error("[PhysicsScene] Failed to create PhysicsScene: PxDefaultCpuDispatcherCreate Failed!\n");
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
				Log.Error("[PhysicsScene] PhysX failed to create PhysicsScene!\n");
				return false;
			}
			scene->scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
			scene->scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			return true;
		}
		void PhysicsScene::Delete(PhysicsScene* scene)
		{

		}

		physx::PxScene * PhysicsScene::GetBase()
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

		bool PhysicsScene::Create(PhysicsScene* scene, const PhysicsSceneDesc& desc)
		{

			return false;
		}
		void PhysicsScene::Delete(PhysicsScene* scene)
		{

		}
		physx::PxScene * PhysicsScene::GetBase()
		{
			return nullptr;
		}
	}
}
#endif