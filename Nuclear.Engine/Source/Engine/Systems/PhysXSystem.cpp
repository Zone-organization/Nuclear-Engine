#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include "..\PhysX\PhysXTypes.h"
#include <PhysX/include/PxPhysicsAPI.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear
{
	namespace Systems
	{
		PhysXSystem::PhysXSystem(const PhysXSystemDesc &sceneDesc)
		{
			
			PhysX::PxSceneDesc SceneDesc(PhysX::PhysXEngine::GetInstance().GetPhysics()->getTolerancesScale());
			SceneDesc.gravity = PhysX::To(sceneDesc.mGravity);
			SceneDesc.cpuDispatcher = PhysX::PhysXEngine::GetInstance().GetCPUDispatcher();
			SceneDesc.filterShader = PhysX::PxDefaultSimulationFilterShader;
	
			mPhysXScene = PhysX::PhysXEngine::GetInstance().GetPhysics()->createScene(SceneDesc);
		}
		PhysXSystem::~PhysXSystem()
		{
			mPhysXScene->release();
		}

		void PhysXSystem::SetColliderForRigidBody(ECS::Entity entity)
		{
			//Check if entity is valid
			auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

			if (RigidComponent && ColliderComponent)
			{
				RigidComponent->GetActor().mPtr->attachShape(*ColliderComponent->GetShape().mPtr);
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
					RigidComponent->GetActor().mPtr->attachShape(*Obj.GetShape().mPtr);
					mPhysXScene->addActor(*RigidComponent->GetActor().mPtr);
				}
				else {
					mPhysXScene->addActor(*Obj.GetActor().mPtr);
				}
			}
		}

		void PhysXSystem::AddunAssignedActors()
		{
			auto eView = mScene->GetRegistry().view<Components::ColliderComponent>();
			for (auto entity : eView)
			{
				auto& Obj = eView.get<Components::ColliderComponent>(entity);
				if (!Obj.isValid())
				{
					auto RigidComponent = mScene->GetRegistry().try_get<Components::RigidBodyComponent>(entity);
					if (RigidComponent)
					{
						RigidComponent->GetActor().mPtr->attachShape(*Obj.GetShape().mPtr);
						mPhysXScene->addActor(*RigidComponent->GetActor().mPtr);
					}
					else {
						mPhysXScene->addActor(*Obj.GetActor().mPtr);
					}

					Obj.SetValid(true);
				}
			}
		}

		bool PhysXSystem::AddActor(ECS::Entity entity)
		{
			auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			auto ColliderComponent = entity.GetComponent<Components::ColliderComponent>();

			if (RigidComponent && ColliderComponent)
			{
				mPhysXScene->addActor(*RigidComponent->GetActor().mPtr);
				RigidComponent->GetActor().mPtr->userData = (void*)entity.entity;
				ColliderComponent->GetActor().mPtr->userData = (void*)entity.entity;
				return true;
			}

			if (ColliderComponent)
			{
				mPhysXScene->addActor(*ColliderComponent->GetActor().mPtr);
				ColliderComponent->GetActor().mPtr->userData = (void*)entity.entity;
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
				outhit.HitEntity.parent = &mScene->GetRegistry();
				outhit.HitEntity.entity = (entt::entity&)hit.block.actor->userData;
					
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
				auto Einfo = mScene->GetRegistry().try_get<Components::EntityInfoComponent>(entity);

				if (!RigidBodyObj.isKinematic)
				{

					Einfo->mTransform.SetPosition(PhysX::From(RigidBodyObj.GetActor().mPtr->getGlobalPose().p));
					Einfo->mTransform.SetRotation(PhysX::From(RigidBodyObj.GetActor().mPtr->getGlobalPose().q));
				}
				else {
					PhysX::PxTransform transform(PhysX::To(Einfo->mTransform.GetLocalPosition()), PhysX::To(Einfo->mTransform.GetLocalRotation()));

					RigidBodyObj.GetActor().mPtr->setGlobalPose(transform);
				}
			}
		}
	}
}