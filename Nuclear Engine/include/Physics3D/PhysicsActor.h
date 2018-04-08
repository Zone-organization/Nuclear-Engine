#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
namespace physx { class PxActor; }

namespace NuclearEngine
{
	namespace Physics3D
	{

		class NEAPI PhysicsActor
		{
		public:
			static bool Create(PhysicsActor* actor);
			static void Delete(PhysicsActor* actor);

			physx::PxActor* GetBase();
		private:
			physx::PxActor* actor;
		};
	}
}