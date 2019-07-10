#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/PhysX/PhysXEngine.h>

namespace NuclearEngine
{
	namespace Components
	{
		enum Shape
		{
			COLLIDER_SHAPE_BOX,
			COLLIFER_SHAPE_PLANE
		};

		class NEAPI ColliderComponent : public ECS::Component<ColliderComponent>
		{
		public:
			ColliderComponent();
			~ColliderComponent();

			PhysX::PxShape* mShape = nullptr;
			PhysX::PxMaterial* mMaterial = nullptr;
			PhysX::PxRigidStatic* mStaticActor = nullptr;
		};
	}
}