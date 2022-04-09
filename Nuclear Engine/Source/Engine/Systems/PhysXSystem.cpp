#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include "..\PhysX\PhysXTypes.h"
#include <PhysX/include/PxPhysicsAPI.h>

namespace NuclearEngine
{
	namespace Systems
	{
		PhysXSystem::PhysXSystem(const PhysXSystemDesc &sceneDesc)
		{
			
			PhysX::PxSceneDesc SceneDesc(PhysX::PhysXEngine::GetPhysics()->getTolerancesScale());
			SceneDesc.gravity = PhysX::To(sceneDesc.mGravity);
			SceneDesc.cpuDispatcher = PhysX::PhysXEngine::GetCPUDispatcher();
			SceneDesc.filterShader = PhysX::PxDefaultSimulationFilterShader;
	
			mPhysXScene = PhysX::PhysXEngine::GetPhysics()->createScene(SceneDesc);
		}
		PhysXSystem::~PhysXSystem()
		{
		}

		void PhysXSystem::SetColliderForRigidBody(ECS::Entity entity)
		{
			//Check if entity is valid
			auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

			if (RigidComponent && ColliderComponent)
			{
				RigidComponent->mDynamicActor.mPtr->attachShape(*ColliderComponent->mShape.mPtr);
			}
		}
		
		physx::PxScene* PhysXSystem::GetPhysXScene()
		{
			return mPhysXScene;
		}

		void PhysXSystem::SetPhysXScene(physx::PxScene* scene)
		{
			mPhysXScene = scene;
		}

		void PhysXSystem::BeginSimulation(ECS::TimeDelta dt)
		{
			mPhysXScene->simulate(dt);
		}
		void PhysXSystem::Bake()
		{
			auto eView = mScene->GetRegistry().view<Components::ColliderComponent>();
			for (auto entity : eView)
			{
				auto& Obj = eView.get<Components::ColliderComponent>(entity);

				auto RigidComponent = mScene->GetRegistry().try_get<Components::RigidBodyComponent>(entity);
				if (RigidComponent)
				{		
					RigidComponent->mDynamicActor.mPtr->attachShape(*Obj.mShape.mPtr);
					mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
				}
				else {
					mPhysXScene->addActor(*Obj.mStaticActor.mPtr);
				}
			}
		}

		void PhysXSystem::AddunAssignedActors()
		{
			auto eView = mScene->GetRegistry().view<Components::ColliderComponent>();
			for (auto entity : eView)
			{
				auto& Obj = eView.get<Components::ColliderComponent>(entity);
				if (!Obj.mAddedtoPhysxScene)
				{
					auto RigidComponent = mScene->GetRegistry().try_get<Components::RigidBodyComponent>(entity);
					if (RigidComponent)
					{
						RigidComponent->mDynamicActor.mPtr->attachShape(*Obj.mShape.mPtr);
						mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
					}
					else {
						mPhysXScene->addActor(*Obj.mStaticActor.mPtr);
					}

					Obj.mAddedtoPhysxScene = true;
				}
			}
		}

		bool PhysXSystem::AddActor(ECS::Entity entity)
		{
			auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

			if (RigidComponent && ColliderComponent)
			{
				mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
				RigidComponent->mDynamicActor.mPtr->userData = (void*)entity.entity;
				ColliderComponent->mStaticActor.mPtr->userData = (void*)entity.entity;
				return true;
			}

			if (ColliderComponent)
			{
				mPhysXScene->addActor(*ColliderComponent->mStaticActor.mPtr);
				ColliderComponent->mStaticActor.mPtr->userData = (void*)entity.entity;
				return true;
			}

			return false;
		}

		//bool PhysXSystem::Raycast()
		//{
		//	PxReal
		///*	const PxVec3& origin;
		//	const PxVec3& unitDir;
		//		const PxReal 	distance,
		//		PxRaycastCallback& hitCall,
		//		PxHitFlags 	hitFlags = PxHitFlags(PxHitFlag::eDEFAULT),
		//		const PxQueryFilterData& filterData = PxQueryFilterData(),
		//		PxQueryFilterCallback* filterCall = NULL,
		//		const PxQueryCache* cache = NULL
		//		*/
		//	return mPhysXScene->raycast();
		//}

		bool PhysXSystem::Raycast(const Math::Vector3& Origin, const Math::Vector3& UnitDir, const Float32& distance, PhysX::RaycastHit& outhit)
		{
			physx::PxRaycastBuffer hit;

			if (mPhysXScene->raycast(PhysX::To(Origin), PhysX::To(UnitDir), distance, hit))
			{
				outhit.position = PhysX::From(hit.block.position);
				outhit.normal = PhysX::From(hit.block.normal);
				outhit.distance = hit.block.distance;
				//(entt::entity)hit.block.actor->userData;

				outhit.HitEntity.parent = &mScene->GetRegistry();
				outhit.HitEntity.entity = (entt::entity&)hit.block.actor->userData;
					
				//outhit.HitEntity = mScene->Entities.Get(ECS::Entity::Id((uint64_t)hit.block.actor->userData));
				outhit.valid = true;
				return true;
			}

			return false;
		}

		void PhysXSystem::Update(ECS::TimeDelta dt)
		{
			mPhysXScene->simulate(dt);
			mPhysXScene->fetchResults(true);

			//Update Entities transforms
			auto eView = mScene->GetRegistry().view<Components::RigidBodyComponent>();
			for (auto entity : eView)
			{
				auto& RigidBodyObj = eView.get<Components::RigidBodyComponent>(entity);

				if (RigidBodyObj.GetInternalisKinematic() != RigidBodyObj.isKinematic)
				{
					RigidBodyObj.SetisKinematic(RigidBodyObj.isKinematic);
				}
				mScene->GetRegistry().try_get<Components::EntityInfoComponent>(entity)->mTransform.SetTransform(PhysX::From(RigidBodyObj.mDynamicActor.mPtr->getGlobalPose()));
			}
		}
	}
}