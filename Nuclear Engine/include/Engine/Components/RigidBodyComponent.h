#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/PhysX/PhysXEngine.h>

#define RigidActorStatic true
#define RigidActorDynamic false

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI RigidBodyComponent : public ECS::Component<RigidBodyComponent>
		{
		public:
			RigidBodyComponent();
			~RigidBodyComponent();

			PhysX::PxRigidDynamic* mDynamicActor = nullptr;
		};
	}
}