#include<Engine\Components\RigidBodyComponent.h>
#include "..\PhysX\PhysXTypes.h"

namespace NuclearEngine
{
	namespace Components
	{
		RigidBodyComponent::RigidBodyComponent()
			:  isKinematic(false), InternalisKinematic(false)
		{

		}

		RigidBodyComponent::RigidBodyComponent(ECS::Transform trans)
			:  isKinematic(false), InternalisKinematic(false)
		{
			mDynamicActor.mPtr = PhysX::PhysXEngine::GetPhysics()->createRigidDynamic(PhysX::To(trans));
		}

		RigidBodyComponent::~RigidBodyComponent()
		{
		}
		void RigidBodyComponent::SetisKinematic(bool value)
		{

			isKinematic = value;
			InternalisKinematic = isKinematic;
			mDynamicActor.mPtr->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);

			if (value == false)
			{
				mDynamicActor.mPtr->wakeUp();
			}
		}
		bool RigidBodyComponent::GetisKinematic()
		{
			return isKinematic;
		}
		bool RigidBodyComponent::GetInternalisKinematic()
		{
			return InternalisKinematic;
		}
	}
}