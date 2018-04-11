#include <Physics3D\PhysicsScene.h>

#if defined (NE_USE_PHYSICS3DENGINE)
#include <Physics3D\Physics3DEngine.h>
#include <PhysX\Include\PxPhysicsAPI.h>
#include "Physics3D\RigidBody.h"
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
		
			scene->scene = Physics3DEngine::GetContext()->createScene(sceneDesc);
			if (!scene->scene)
			{
				Log.Error("[PhysicsScene] PhysX failed to create PhysicsScene!\n");
				return false;
			}

			return true;
		}
		void PhysicsScene::Delete(PhysicsScene* scene)
		{
			scene->scene->release();
		}

		void PhysicsScene::Update(float dt)
		{
			scene->simulate(dt);
		}

		void PhysicsScene::AddActor(RigidBody * actor)
		{
			scene->addActor(*actor->GetBase());
		}

		physx::PxScene * PhysicsScene::GetBase()
		{
			return scene;
		}
	}
}
#endif