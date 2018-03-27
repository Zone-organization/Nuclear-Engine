#pragma once
#include <Physics3D\Physics3DEngine.h>
#include <PhysX\PhysXincluder.h>

#if !defined(PHYSX_NOT_INCLUDED) && defined(NE_USE_PHYSXENGINE)
#include <PhysX\Include\PxPhysicsAPI.h>
#include <Math\Math.h>

using namespace physx;

namespace NuclearEngine
{
	namespace Physics3D
	{
		inline PxVec3 Convert(Math::Vector3 convertable)
		{
			return PxVec3(convertable.x, convertable.y, convertable.z);
		}
	}
}
#endif