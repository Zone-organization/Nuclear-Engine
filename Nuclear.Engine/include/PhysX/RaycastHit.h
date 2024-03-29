#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <ECS\ECS.h>

namespace Nuclear
{
	namespace PhysX
	{
		//TODO : improve
		class NEAPI RaycastHit
		{
		public:
			RaycastHit();
			~RaycastHit();

			bool valid;

			Math::Vector3 position;
			Math::Vector3 normal;
			Float32	distance;

			ECS::Entity HitEntity;
		};
	}
}