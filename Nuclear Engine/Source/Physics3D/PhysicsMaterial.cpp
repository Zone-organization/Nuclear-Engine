#include <Physics3D\PhysicsMaterial.h>

#if defined (NE_USE_PHYSICS3DENGINE)
#include <Physics3D\Physics3DEngine.h>
#include <PhysX\Include\PxPhysicsAPI.h>
#include "PhysXCommon.h"
using namespace physx;

namespace NuclearEngine
{
	namespace Physics3D
	{
		bool PhysicsMaterial::Create(PhysicsMaterial* Mat, float staticFriction, float dynamicFriction, float restitution)
		{
			Mat->material = Physics3DEngine::GetContext()->createMaterial(staticFriction, dynamicFriction, restitution);
			
			return true;
		}
		void PhysicsMaterial::Delete(PhysicsMaterial* Mat)
		{
			Mat->material->release();
		}

		physx::PxMaterial * PhysicsMaterial::GetBase()
		{
			return material;
		}
	}
}
#endif