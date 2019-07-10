#include <Engine\Systems\PhysXSystem.h>

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
		void PhysXSystem::BeginSimulation(ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->simulate(dt);
		}
		void PhysXSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{
			mScene->GetPhysXScene()->fetchResults(false);
		}
	}
}