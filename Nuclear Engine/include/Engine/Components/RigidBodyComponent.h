#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine\ECS\Transform.h>
#include <Engine/PhysX/PhysXEngine.h>
#include <Engine/PhysX/RigidDynamic.h>

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
			RigidBodyComponent(ECS::Transform trans);
			~RigidBodyComponent();

			PhysX::RigidDynamic mDynamicActor;

			bool isKinematic;

			void SetisKinematic(bool value);

			bool GetisKinematic();

			bool GetInternalisKinematic();
		private: 
			bool InternalisKinematic;
		};
	}
}