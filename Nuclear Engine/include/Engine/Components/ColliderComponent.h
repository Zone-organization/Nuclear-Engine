#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/PhysX/PhysXEngine.h>
#include <Engine/PhysX/PhysXMaterial.h>
#include <Engine/PhysX/PhysXShape.h>
#include <Engine/PhysX/RigidStatic.h>
#include <Engine/PhysX/Geometry.h>

namespace NuclearEngine
{
	namespace Components
	{
		enum COLLIDER_SHAPE
		{
			COLLIDER_SHAPE_UNKNOWN,
			COLLIDER_SHAPE_BOX,
			COLLIDER_SHAPE_PLANE,
			COLLIDER_SHAPE_SPHERE
		};

		class NEAPI ColliderComponent : public ECS::Component<ColliderComponent>
		{
		public:
			ColliderComponent();
			ColliderComponent(PhysX::PhysXMaterial* PMat, PhysX::BoxGeometry& Geo);
			ColliderComponent(PhysX::PhysXMaterial* PMat, PhysX::PlaneGeometry& Geo);
			ColliderComponent(PhysX::PhysXMaterial* PMat, PhysX::SphereGeometry& Geo);
			~ColliderComponent();

			COLLIDER_SHAPE mType = COLLIDER_SHAPE_UNKNOWN;
			PhysX::PhysXMaterial* mMaterial;
			PhysX::PhysXShape mShape;
			PhysX::RigidStatic mStaticActor;
			bool mAddedtoPhysxScene = false;
		};
	}
}