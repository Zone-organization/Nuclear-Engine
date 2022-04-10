#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine\ECS\Transform.h>
#include <Engine/PhysX/PhysXEngine.h>
#include <Engine/PhysX/RigidDynamic.h>

#define RigidActorStatic true
#define RigidActorDynamic false

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI RigidBodyComponent
		{
		public:
			RigidBodyComponent();
			RigidBodyComponent(ECS::Transform trans);
			~RigidBodyComponent();

			PhysX::RigidDynamic& GetActor();

			bool isKinematic;

			void SetisKinematic(bool value);

			bool GetisKinematic();

			bool GetInternalisKinematic();
		private: 
			PhysX::RigidDynamic mDynamicActor;

			bool InternalisKinematic;
		};
	}
}