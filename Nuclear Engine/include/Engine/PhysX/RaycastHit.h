#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\ECS.h>

namespace NuclearEngine
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