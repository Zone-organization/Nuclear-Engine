#include<Components\RigidBodyComponent.h>
#include "..\PhysX\PhysXTypes.h"

namespace Nuclear
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
			mDynamicActor.mPtr = PhysX::PhysXModule::GetInstance().GetPhysics()->createRigidDynamic(PhysX::To(trans));
		}

		RigidBodyComponent::~RigidBodyComponent()
		{
		}
		PhysX::RigidDynamic& RigidBodyComponent::GetActor()
		{
			return mDynamicActor;
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