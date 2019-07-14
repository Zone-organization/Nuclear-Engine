#include<Engine\Components\RigidBodyComponent.h>
#include "..\PhysX\PhysXTypes.h"

namespace NuclearEngine
{
	namespace Components
	{
		RigidBodyComponent::RigidBodyComponent()
		{

		}

		RigidBodyComponent::RigidBodyComponent(ECS::Transform trans)
		{
			mDynamicActor.mPtr = PhysX::PhysXEngine::GetPhysics()->createRigidDynamic(PhysX::To(trans));
		}

		RigidBodyComponent::~RigidBodyComponent()
		{
		}
	}
}