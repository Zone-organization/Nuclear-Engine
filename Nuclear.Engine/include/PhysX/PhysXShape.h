#pragma once
#include <NE_Common.h>

namespace physx { class PxShape; }

namespace Nuclear
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