#pragma once
#include <Base/NE_Common.h>
#include <Base/Math/Math.h>
#include <Engine/ECS/Transform.h>

namespace NuclearEngine
{
	namespace PhysX
	{
		class NEAPI BoxGeometry
		{
		public:			
			BoxGeometry(Math::Vector3 halfExtents_, ECS::Transform& Transform_) : mTransform(Transform_), mHalfExtents(halfExtents_) {}

			Math::Vector3 mHalfExtents;
			ECS::Transform mTransform;
		};

		class NEAPI PlaneGeometry
		{
		public:

			PlaneGeometry(ECS::Transform Transform_) : mTransform(Transform_) {}

			ECS::Transform mTransform;
		};

		class NEAPI SphereGeometry
		{
		public:

			SphereGeometry(float radius_, ECS::Transform Transform_) : radius(radius_), mTransform(Transform_) {}

			float radius;
			ECS::Transform mTransform;
		};

	}
}