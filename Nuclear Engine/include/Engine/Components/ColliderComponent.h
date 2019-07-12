#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/PhysX/PhysXEngine.h>
#include <Engine/PhysX/PhysXMaterial.h>

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
			ColliderComponent(PhysX::PhysXMaterial Pmat);
			~ColliderComponent();

			PhysX::PhysXMaterial mMaterial;

			PhysX::PxShape* mShape = nullptr;
			PhysX::PxRigidStatic* mStaticActor = nullptr;
		};
	}
}