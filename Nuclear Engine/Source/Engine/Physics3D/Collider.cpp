#include <Engine\Physics3D\Collider.h>

#if defined (NE_USE_PHYSICS3DENGINE)
#include <Engine\Physics3D\Physics3DEngine.h>
#include "PhysXCommon.h"
using namespace physx;

namespace NuclearEngine
{
	namespace Physics3D
	{
		bool Collider::Create(Collider* collider)
		{

			return true;
		}
		void Collider::Delete(Collider* collider)
		{
			collider->mBase->release();
		}

		Math::Vector3 Collider::GetPosition() const
		{
			return ConvertFrom(mBase->getLocalPose().p);
		}

		Math::Quaternion Collider::GetRotation() const
		{
			return ConvertFrom(mBase->getLocalPose().q);
		}

		void Collider::SetTransform(const Math::Vector3& pos, const Math::Quaternion& rot)
		{
			mBase->setLocalPose(Convert(pos, rot));
		}

		void Collider::SetIsTrigger(bool value)
		{
			if (value)
			{
				mBase->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
				mBase->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

				isTrigger = true;
			}
			else
			{
				mBase->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
				mBase->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

				isTrigger = false;
			}
		}

		bool Collider::GetIsTrigger() const
		{
			return isTrigger;
		}

		void Collider::SetRigidBody(RigidBody* value)
		{
			mRigidBody = value;
		}

		RigidBody * Collider::GetRigidBody()
		{
			return mRigidBody;
		}

		
		void Collider::SetMaterial(const PhysicsMaterial& material)
		{
			mMaterial = material;
		}

		PhysicsMaterial Collider::GetMaterial() const
		{
			return mMaterial;
		}	

		physx::PxShape * Collider::GetBase()
		{
			return mBase;
		}
	}
}
#endif