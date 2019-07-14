#include "Engine\Components\ColliderComponent.h"
#include <PhysX/include/PxPhysicsAPI.h>
#include "..\PhysX\PhysXTypes.h"

namespace NuclearEngine
{
	namespace Components
	{
		ColliderComponent::ColliderComponent()
		{
		}
		ColliderComponent::ColliderComponent(PhysX::PhysXMaterial PMat, PhysX::BoxGeometry& Geo)
		{		
			mMaterial = PMat;
			mShape.mPtr = PhysX::PhysXEngine::GetPhysics()->createShape(physx::PxBoxGeometry(Geo.mHalfExtents.x, Geo.mHalfExtents.y, Geo.mHalfExtents.z), *mMaterial.GetPtr());	
			mStaticActor.mPtr = PhysX::PhysXEngine::GetPhysics()->createRigidStatic(PhysX::To(Geo.mTransform));
			mStaticActor.mPtr->attachShape(*mShape.mPtr);

			//mScene->GetPhysXScene()->addActor(*mStaticActor.mPtr);
		}

		ColliderComponent::ColliderComponent(PhysX::PhysXMaterial PMat, PhysX::PlaneGeometry& Geo)
		{
			mMaterial = PMat;
			mStaticActor.mPtr = PxCreatePlane(*PhysX::PhysXEngine::GetPhysics(), PhysX::PxPlane(0, 1, 0, 0), *mMaterial.GetPtr());
		}

		ColliderComponent::~ColliderComponent()
		{
		}
	}
}