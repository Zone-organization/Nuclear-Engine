#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>
#include <Engine\ECS\Transform.h>
#include "Engine\PhysX\PhysXEngine.h"
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\ColliderComponent.h>

namespace NuclearEngine
{
	namespace Systems
	{

		struct PhysXSystemDesc
		{
			Math::Vector3 mGravity;
		};

		class NEAPI PhysXSystem : public ECS::System<PhysXSystem> {
		public:
			PhysXSystem(ECS::Scene* scene,const PhysXSystemDesc& sceneDesc);
			~PhysXSystem();

			void SetColliderForRigidBody(ECS::Entity entity);

			//Must Be called before update!
			void BeginSimulation(ECS::TimeDelta dt);

			void Bake(ECS::EntityManager& es);

			void AddunAssignedActors(ECS::EntityManager& es);

			bool AddActor(ECS::Entity entity);

			//void AddActor(Components::ColliderComponent collider);

			//void AddActor(Components::RigidBodyComponent rigidbody);

			//void AddActor(Components::RigidBodyComponent rigidbody);

			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:
			ECS::Scene* mScene = nullptr;
		};

	}
}