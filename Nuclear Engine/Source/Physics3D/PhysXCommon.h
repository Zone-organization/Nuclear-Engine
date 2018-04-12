#pragma once
#include <Physics3D\Physics3DEngine.h>
#if defined (NE_USE_PHYSICS3DENGINE)
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

		inline PxVec4 Convert(Math::Vector4 convertable)
		{
			return PxVec4(convertable.x, convertable.y, convertable.z, convertable.w);
		}

		inline PxQuat Convert(Math::Quaternion convertable)
		{
			return PxQuat(convertable.x, convertable.y, convertable.z, convertable.w);
		}
		
		inline PxTransform Convert(Math::Vector3 avec, Math::Quaternion aquat)
		{
			return PxTransform(Convert(avec),Convert(aquat));
		}

		inline Math::Vector3 ConvertFrom(PxVec3 convertable)
		{
			return Math::Vector3(convertable.x, convertable.y, convertable.z);
		}

		inline Math::Vector4 ConvertFrom(PxVec4 convertable)
		{
			return Math::Vector4(convertable.x, convertable.y, convertable.z, convertable.w);
		}

		inline Math::Quaternion ConvertFrom(PxQuat convertable)
		{
			return Math::Quaternion(convertable.x, convertable.y, convertable.z, convertable.w);
		}

	}
}
#endif