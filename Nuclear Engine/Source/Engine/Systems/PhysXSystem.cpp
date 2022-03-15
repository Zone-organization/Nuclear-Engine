#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include "..\PhysX\PhysXTypes.h"

namespace NuclearEngine
{
	namespace Systems
	{
		PhysXSystem::PhysXSystem(ECS::Scene* scene,const PhysXSystemDesc &sceneDesc)
			: mScene(scene)
		{
			
			PhysX::PxSceneDesc SceneDesc(PhysX::PhysXEngine::GetPhysics()->getTolerancesScale());
			SceneDesc.gravity = PhysX::To(sceneDesc.mGravity);
			SceneDesc.cpuDispatcher = PhysX::PhysXEngine::GetCPUDispatcher();
			SceneDesc.filterShader = PhysX::PxDefaultSimulationFilterShader;
	
			mScene->SetPhysXScene(PhysX::PhysXEngine::GetPhysics()->createScene(SceneDesc));
		}
		PhysXSystem::~PhysXSystem()
		{
		}
		//void PhysXSystem::CreatePlaneCollider(Components::ColliderComponent* Component, const PhysX::PxPlane& plane)
		//{
		//	Component->mStaticActor.mPtr = PxCreatePlane(*PhysX::PhysXEngine::GetPhysics(), plane, *Component->mMaterial.GetPtr());

		//	mScene->GetPhysXScene()->addActor(*Component->mStaticActor.mPtr);
		//}
		//void PhysXSystem::CreateBoxCollider(Components::ColliderComponent* Component, ECS::Transform& t, const PhysX::PxBoxGeometry& geometry)
		//{
		//	Component->mShape.mPtr = PhysX::PhysXEngine::GetPhysics()->createShape(geometry, *Component->mMaterial.GetPtr());
		//	Component->mStaticActor.mPtr = PxCreateStatic(*PhysX::PhysXEngine::GetPhysics(), PhysX::To(t), *Component->mShape.mPtr);
		//	mScene->GetPhysXScene()->addActor(*Component->mStaticActor.mPtr);
		//}
		//void PhysXSystem::CreateRigidBody(Components::RigidBodyComponent* Component, ECS::Transform& t)
		//{
		//	Component->mDynamicActor.mPtr = PhysX::PhysXEngine::GetPhysics()->createRigidDynamic(PhysX::To(t));

		//	mScene->GetPhysXScene()->addActor(*Component->mDynamicActor.mPtr);
		//}

		void PhysXSystem::SetColliderForRigidBody(ECS::Entity entity)
		{
			//Check if entity is valid
			if (entity.Valid())
			{
				auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
				auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

				if (RigidComponent.Valid() && ColliderComponent.Valid())
				{
					RigidComponent->mDynamicActor.mPtr->attachShape(*ColliderComponent->mShape.mPtr);
				}

			}
		}
		
		void PhysXSystem::BeginSimulation(ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->simulate(dt);
		}
		void PhysXSystem::Bake(ECS::EntityManager& es)
		{
			//ECS::ComponentHandle<Components::ColliderComponent> Obj;
			//for (ECS::Entity entity : es.entities_with_components(Obj))
			//{
			//	auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			//	if (RigidComponent.Valid())
			//	{		
			//		RigidComponent->mDynamicActor.mPtr->attachShape(*Obj->mShape.mPtr);
			//		mScene->GetPhysXScene()->addActor(*RigidComponent->mDynamicActor.mPtr);
			//	}
			//	else {
			//		mScene->GetPhysXScene()->addActor(*Obj->mStaticActor.mPtr);
			//	}
			//}
		}

		void PhysXSystem::AddunAssignedActors(ECS::EntityManager& es)
		{
			ECS::ComponentHandle<Components::ColliderComponent> Obj;
			for (ECS::Entity entity : es.entities_with_components(Obj))
			{
				if (!Obj->mAddedtoPhysxScene)
				{
					auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
					if (RigidComponent.Valid())
					{
						RigidComponent->mDynamicActor.mPtr->attachShape(*Obj->mShape.mPtr);
						mScene->GetPhysXScene()->addActor(*RigidComponent->mDynamicActor.mPtr);
					}
					else {
						mScene->GetPhysXScene()->addActor(*Obj->mStaticActor.mPtr);
					}
					Obj->mAddedtoPhysxScene = true;
				}
			}
		}

		bool PhysXSystem::AddActor(ECS::Entity entity)
		{
			//Check if entity is valid
			if (entity.Valid())
			{
				auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
				auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

				if (RigidComponent.Valid() && ColliderComponent.Valid())
				{
					mScene->GetPhysXScene()->addActor(*RigidComponent->mDynamicActor.mPtr);

					return true;
				}

				if (ColliderComponent.Valid())
				{
					mScene->GetPhysXScene()->addActor(*ColliderComponent->mStaticActor.mPtr);

					return true;
				}
			}
			return false;
		}

		void PhysXSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->fetchResults(true);

			//Update Entities transforms
			ECS::ComponentHandle<Components::RigidBodyComponent> RigidBodyObj;
			for (ECS::Entity entity : es.entities_with_components(RigidBodyObj))
			{
				if (RigidBodyObj.Get()->GetInternalisKinematic() != RigidBodyObj.Get()->isKinematic)
				{
					RigidBodyObj.Get()->SetisKinematic(RigidBodyObj.Get()->isKinematic);
				}

				entity.GetComponent<Components::EntityInfoComponent>().Get()->mTransform.SetTransform(PhysX::From(RigidBodyObj->mDynamicActor.mPtr->getGlobalPose()));
			}
		}
	}
}