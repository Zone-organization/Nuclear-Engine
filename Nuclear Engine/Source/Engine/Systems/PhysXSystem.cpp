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
		PhysXSystem::PhysXSystem(ECS::Scene* scene,const PhysXSystemDesc &sceneDesc)
			: mScene(scene)
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
		//void PhysXSystem::CreatePlaneCollider(Components::ColliderComponent* Component, const PhysX::PxPlane& plane)
		//{
		//	Component->mStaticActor.mPtr = PxCreatePlane(*PhysX::PhysXEngine::GetPhysics(), plane, *Component->mMaterial.GetPtr());

		//	mPhysXScene->addActor(*Component->mStaticActor.mPtr);
		//}
		//void PhysXSystem::CreateBoxCollider(Components::ColliderComponent* Component, ECS::Transform& t, const PhysX::PxBoxGeometry& geometry)
		//{
		//	Component->mShape.mPtr = PhysX::PhysXEngine::GetPhysics()->createShape(geometry, *Component->mMaterial.GetPtr());
		//	Component->mStaticActor.mPtr = PxCreateStatic(*PhysX::PhysXEngine::GetPhysics(), PhysX::To(t), *Component->mShape.mPtr);
		//	mPhysXScene->addActor(*Component->mStaticActor.mPtr);
		//}
		//void PhysXSystem::CreateRigidBody(Components::RigidBodyComponent* Component, ECS::Transform& t)
		//{
		//	Component->mDynamicActor.mPtr = PhysX::PhysXEngine::GetPhysics()->createRigidDynamic(PhysX::To(t));

		//	mPhysXScene->addActor(*Component->mDynamicActor.mPtr);
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
		void PhysXSystem::Bake(ECS::EntityManager& es)
		{
			//ECS::ComponentHandle<Components::ColliderComponent> Obj;
			//for (ECS::Entity entity : es.entities_with_components(Obj))
			//{
			//	auto RigidComponent = entity.GetComponent<Components::RigidBodyComponent>();
			//	if (RigidComponent.Valid())
			//	{		
			//		RigidComponent->mDynamicActor.mPtr->attachShape(*Obj->mShape.mPtr);
			//		mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
			//	}
			//	else {
			//		mPhysXScene->addActor(*Obj->mStaticActor.mPtr);
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
						mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
					}
					else {
						mPhysXScene->addActor(*Obj->mStaticActor.mPtr);
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
					mPhysXScene->addActor(*RigidComponent->mDynamicActor.mPtr);
					RigidComponent->mDynamicActor.mPtr->userData = (void*)entity.id().id();
					ColliderComponent->mStaticActor.mPtr->userData = (void*)entity.id().id();
					return true;
				}

				if (ColliderComponent.Valid())
				{
					mPhysXScene->addActor(*ColliderComponent->mStaticActor.mPtr);
					ColliderComponent->mStaticActor.mPtr->userData = (void*)entity.id().id();
					return true;
				}
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
				outhit.HitEntity = mScene->Entities.Get(ECS::Entity::Id((uint64_t)hit.block.actor->userData));
				outhit.valid = true;
				return true;
			}

			return false;
		}

		void PhysXSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{

			mPhysXScene->fetchResults(true);

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