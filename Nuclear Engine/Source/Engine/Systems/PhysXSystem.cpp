#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\TransformComponent.h>
#include <Engine/PhysX/PhysXTypes.h>

namespace NuclearEngine
{
	namespace Systems
	{
		PhysXSystem::PhysXSystem(ECS::Scene* scene, PhysX::PxSceneDesc sceneDesc)
			: mScene(scene)
		{
	
			mScene->SetPhysXScene(PhysX::PhysXEngine::GetPhysics()->createScene(sceneDesc));
		}
		PhysXSystem::~PhysXSystem()
		{
		}
		void PhysXSystem::CreatePlaneCollider(Components::ColliderComponent* Component, const PhysX::PxPlane& plane)
		{
			Component->mStaticActor = PxCreatePlane(*PhysX::PhysXEngine::GetPhysics(), plane, *Component->mMaterial);

			mScene->GetPhysXScene()->addActor(*Component->mStaticActor);
		}
		void PhysXSystem::CreateBoxCollider(Components::ColliderComponent* Component, const PhysX::PxTransform& t, const PhysX::PxBoxGeometry& geometry)
		{
			Component->mShape = PhysX::PhysXEngine::GetPhysics()->createShape(geometry, *Component->mMaterial);
			Component->mStaticActor = PxCreateStatic(*PhysX::PhysXEngine::GetPhysics(), t, *Component->mShape);
			mScene->GetPhysXScene()->addActor(*Component->mStaticActor);
		}
		void PhysXSystem::CreateRigidBody(Components::RigidBodyComponent* Component, const PhysX::PxTransform& t)
		{
			Component->mDynamicActor = PhysX::PhysXEngine::GetPhysics()->createRigidDynamic(t);

			mScene->GetPhysXScene()->addActor(*Component->mDynamicActor);
		}
		
		void PhysXSystem::BeginSimulation(ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->simulate(dt);
		}
		void PhysXSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->fetchResults(true);

			//Update Entities transforms
			ECS::ComponentHandle<Components::RigidBodyComponent> RigidBodyObj;
			for (ECS::Entity entity : es.entities_with_components(RigidBodyObj))
			{
				entity.GetComponent<Components::TransformComponent>().Get()->SetTransform(PhysX::From(RigidBodyObj->mDynamicActor->getGlobalPose()));
			}
		}
	}
}