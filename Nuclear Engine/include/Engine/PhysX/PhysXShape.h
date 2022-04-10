#pragma once
#include <Core\NE_Common.h>

namespace physx { class PxShape; }

namespace NuclearEngine
{
	namespace PhysX
	{
		//Wraps around physx::PxShape
		class NEAPI PhysXShape
		{
		public:
			PhysXShape();
			~PhysXShape();


			physx::PxShape* mPtr;
		};
	}
}