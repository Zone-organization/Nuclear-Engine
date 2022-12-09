#pragma once
#include <NE_Common.h>
#include <ECS/ECS.h>
#include "PhysX\PhysXEngine.h"
#include "PhysX\RaycastHit.h"
#include <Components\RigidBodyComponent.h>
#include <Components\ColliderComponent.h>

namespace physx { class PxScene; };

namespace Nuclear
{
	namespace Systems
	{

		struct PhysXSystemDesc
		{
			Math::Vector3 mGravity;
		};

		class NEAPI PhysXSystem : public ECS::System<PhysXSystem>
		{
		public:
			PhysXSystem(const PhysXSystemDesc& sceneDesc);
			~PhysXSystem();

			void SetColliderForRigidBody(ECS::Entity entity);

			//Must Be called before update!
			physx::PxScene* GetPhysXScene();
			void SetPhysXScene(physx::PxScene* scene);

			void BeginSimulation(ECS::TimeDelta dt);

			void Bake();

			void AddunAssignedActors();

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

			void Update(ECS::TimeDelta dt) override;
		private:
			physx::PxScene* mPhysXScene;
		};

	}
}