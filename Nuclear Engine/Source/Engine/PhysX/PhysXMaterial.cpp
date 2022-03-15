#include <Engine\PhysX\PhysXMaterial.h>
#include <PhysX/include/PxPhysicsAPI.h>
#include "Engine\PhysX\PhysXEngine.h"

namespace NuclearEngine
{
	namespace PhysX
	{
		PhysXMaterial::PhysXMaterial()
			: mMaterial(nullptr)
		{
		}
		PhysXMaterial::~PhysXMaterial()
		{
			if (mMaterial)
			{
				mMaterial->release();
			}
		}
		void PhysXMaterial::Create(float staticFriction, float dynamicFriction, float restitution)
		{
			mMaterial = PhysXEngine::GetPhysics()->createMaterial(staticFriction, dynamicFriction, restitution);
		}
		void PhysXMaterial::SetStaticFriction(float value)
		{
			mMaterial->setStaticFriction(value);
		}
		float PhysXMaterial::GetStaticFriction() const
		{
			return mMaterial->getStaticFriction();
		}
		void PhysXMaterial::SetDynamicFriction(float value)
		{
			mMaterial->setDynamicFriction(value);
		}
		float PhysXMaterial::GetDynamicFriction() const
		{
			return mMaterial->getDynamicFriction();
		}
		void PhysXMaterial::SetRestitution(float value)
		{
			mMaterial->setRestitution(value);
		}
		float PhysXMaterial::GetRestitution() const
		{
			return mMaterial->getRestitution();
		}
		physx::PxMaterial* PhysXMaterial::GetPtr()
		{
			return mMaterial;
		}
	}
}