#pragma once
#include <ECS\Transform.h>
#include <PhysX/PhysXEngine.h>
#include <PhysX/RigidDynamic.h>

#define RigidActorStatic true
#define RigidActorDynamic false

namespace Nuclear
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