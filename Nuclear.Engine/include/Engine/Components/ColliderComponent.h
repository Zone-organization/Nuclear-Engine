#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine/PhysX/PhysXEngine.h>
#include <Engine/PhysX/PhysXMaterial.h>
#include <Engine/PhysX/PhysXShape.h>
#include <Engine/PhysX/RigidStatic.h>
#include <Engine/PhysX/Geometry.h>

namespace Nuclear
{
	namespace Components
	{
		enum class ColliderShape
		{
			Unknown,
			Box,
			Plane,
			Sphere
		};

		struct NEAPI ColliderDesc
		{
			float mStaticFriction = 0.5f;
			float mDynamicFriction = 0.5f;
			float mRestitution = 0.5f;
			ColliderShape mShape = ColliderShape::Unknown;
			PhysX::PhysicsGeometry* mGeo = nullptr;
		};

		class NEAPI ColliderComponent
		{
		public:
			ColliderComponent();
			ColliderComponent(const ColliderDesc& desc);
			~ColliderComponent();

			void Create(const ColliderDesc& desc);
			bool isValid();
			void SetValid(bool value);

			PhysX::PhysXShape& GetShape();
			PhysX::RigidStatic& GetActor();
		private:
			ColliderShape mType = ColliderShape::Unknown;
			//PhysX::PhysXMaterial* mMaterial;
			PhysX::PhysXShape mShape;
			PhysX::RigidStatic mStaticActor;
			bool mAddedtoPhysxScene = false;
		};
	}
}