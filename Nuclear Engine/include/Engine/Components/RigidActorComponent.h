#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/PhysX/PhysXEngine.h>

namespace NuclearEngine
{
	namespace Components
	{
#define RigidActorStatic true
#define RigidActorDynamic false

		class NEAPI RigidActorComponent : public ECS::Component<RigidActorComponent>
		{
		public:
			RigidActorComponent();
			~RigidActorComponent();

		private:
			PhysX::PxRigidStatic* mPtr;
			bool Type = RigidActorStatic;
		};
	}
}