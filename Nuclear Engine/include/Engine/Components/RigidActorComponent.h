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
		class NEAPI RigidActorComponent : public ECS::Component<RigidActorComponent>
		{
		public:
			RigidActorComponent();
			~RigidActorComponent();

			bool GetType();
			PhysX::PxRigidActor* GetActor();
			PhysX::PxMaterial* GetMaterial();

			PhysX::PxMaterial* MaterialPtr;
			PhysX::PxRigidStatic* StaticPtr;
			PhysX::PxRigidDynamic* DynamicPtr;
			bool Type = RigidActorStatic;
		};
	}
}