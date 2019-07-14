#pragma once
#include <Base\NE_Common.h>

namespace physx { class PxRigidDynamic; }

namespace NuclearEngine
{
	namespace PhysX
	{
		//Wraps around physx::PxRigidDynamic
		class NEAPI RigidDynamic
		{
		public:
			RigidDynamic();
			~RigidDynamic();

			physx::PxRigidDynamic* mPtr;
		};
	}
}