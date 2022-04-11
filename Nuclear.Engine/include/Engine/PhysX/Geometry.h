#pragma once
#include <Engine/ECS/Transform.h>

namespace Nuclear
{
	namespace PhysX
	{
		class NEAPI PhysicsGeometry
		{
		public:
			PhysicsGeometry(ECS::Transform& Transform_)
				: mTransform(Transform_)
			{

			}
			ECS::Transform mTransform;
		};

		class NEAPI BoxGeometry : public PhysicsGeometry
		{
		public:			
			BoxGeometry(Math::Vector3 halfExtents_, ECS::Transform& Transform_) : PhysicsGeometry(Transform_), mHalfExtents(halfExtents_) {}

			Math::Vector3 mHalfExtents;
		};

		class NEAPI PlaneGeometry : public PhysicsGeometry
		{
		public:

			PlaneGeometry(ECS::Transform Transform_) : PhysicsGeometry(Transform_) {}
		};

		class NEAPI SphereGeometry : public PhysicsGeometry
		{
		public:

			SphereGeometry(float radius_, ECS::Transform Transform_) : radius(radius_), PhysicsGeometry(Transform_) {}

			float radius;
		};

	}
}