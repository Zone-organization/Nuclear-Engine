#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>
#include <Engine\ECS\Transform.h>
#include "Engine\PhysX\PhysXEngine.h"
#include "Engine\PhysX\RaycastHit.h"
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\ColliderComponent.h>

namespace physx { class PxScene; };

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
			physx::PxScene* GetPhysXScene();
			void SetPhysXScene(physx::PxScene* scene);

			void BeginSimulation(ECS::TimeDelta dt);

			void Bake(ECS::EntityManager& es);

			void AddunAssignedActors(ECS::EntityManager& es);

			bool AddActor(ECS::Entity entity);

			/*	const PxVec3& origin;
	const PxVec3& unitDir;
		const PxReal 	distance,
		PxRaycastCallback& hitCall,
		PxHitFlags 	hitFlags = PxHitFlags(PxHitFlag::eDEFAULT),
		const PxQueryFilterData& filterData = PxQueryFilterData(),
		PxQueryFilterCallback* filterCall = NULL,
		const PxQueryCache* cache = NULL
		*/

			bool Raycast(const Math::Vector3& Origin, const Math::Vector3& UnitDir, const Float32& distance,
				PhysX::RaycastHit& hit
			);

			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:
			physx::PxScene* mPhysXScene;

			ECS::Scene* mScene = nullptr;
		};

	}
}