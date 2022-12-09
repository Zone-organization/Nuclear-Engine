#pragma once
#include <ECS/Transform.h>

namespace Nuclear
{
	namespace PhysX
	{
		class NEAPI PhysicsGeometry
		{
		public:
			PhysicsGeometry(const ECS::Transform& Transform_)
				: mTransform(Transform_)
			{

			}
			ECS::Transform mTransform;
		};

		class NEAPI BoxGeometry : public PhysicsGeometry
		{
		public:			
			BoxGeometry(Math::Vector3 halfExtents_, const ECS::Transform& Transform_) : PhysicsGeometry(Transform_), mHalfExtents(halfExtents_) {}

			Math::Vector3 mHalfExtents;
		};

		class NEAPI PlaneGeometry : public PhysicsGeometry
		{
		public:

			PlaneGeometry(const ECS::Transform& Transform_) : PhysicsGeometry(Transform_) {}
		};

		class NEAPI SphereGeometry : public PhysicsGeometry
		{
		public:

			SphereGeometry(float radius_, const ECS::Transform& Transform_) : radius(radius_), PhysicsGeometry(Transform_) {}

			float radius;
		};

	}
}