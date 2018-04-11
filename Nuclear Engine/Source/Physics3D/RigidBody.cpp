#include "Physics3D\RigidBody.h"
#if defined (NE_USE_PHYSICS3DENGINE)

#include "PhysXCommon.h"
namespace NuclearEngine
{
	namespace Physics3D
	{
		bool RigidBody::Create(RigidBody * body)
		{
			body->mBase = Physics3DEngine::GetContext()->createRigidDynamic(PxTransform());

			return true;
		}

		void RigidBody::Delete(RigidBody * body)
		{
			if (body)
			{
				body->mBase->release();
			}
		}

		void RigidBody::Move(const Math::Vector3& position)
		{
			if (GetIsKinematic())
			{
				PxTransform target;
				if (!mBase->getKinematicTarget(target))
					target = PxTransform(PxIdentity);

				target.p = Convert(position);

				mBase->setKinematicTarget(target);
			}
			else
			{
				SetTransform(position, GetRotation());
			}
		}

		void RigidBody::Rotate(const Math::Quaternion& rotation)
		{
			if (GetIsKinematic())
			{
				PxTransform target;
				if (!mBase->getKinematicTarget(target))
					target = PxTransform(PxIdentity);

				target.q = Convert(rotation);

				mBase->setKinematicTarget(target);
			}
			else
			{
				SetTransform(GetPosition(), rotation);
			}
		}

		Math::Vector3 RigidBody::GetPosition() const
		{
			return ConvertFrom(mBase->getGlobalPose().p);
		}

		Math::Quaternion RigidBody::GetRotation() const
		{
			return ConvertFrom(mBase->getGlobalPose().q);
		}

		void RigidBody::SetTransform(const Math::Vector3& pos, const Math::Quaternion& rot)
		{
		//	mBase->setGlobalPose(toPxTransform(pos, rot));
		}

		float RigidBody::GetMass() const
		{
			return mBase->getMass();
		}

		void RigidBody::SetIsKinematic(bool kinematic)
		{
			mBase->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
		}

		bool RigidBody::GetIsKinematic() const
		{
			return ((Uint32)mBase->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC) != 0;
		}

		bool RigidBody::isSleeping() const
		{
			return mBase->isSleeping();
		}

		void RigidBody::Sleep()
		{
			mBase->putToSleep();
		}

		void RigidBody::WakeUp()
		{
			mBase->wakeUp();
		}

		void RigidBody::SetSleepThreshold(float threshold)
		{
			mBase->setSleepThreshold(threshold);
		}

		float RigidBody::GetSleepThreshold() const
		{
			return mBase->getSleepThreshold();
		}

		void RigidBody::SetUseGravity(bool gravity)
		{
			mBase->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravity);
		}

		bool RigidBody::GetUseGravity() const
		{
			return ((Uint32)mBase->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY) == 0;
		}

		void RigidBody::SetVelocity(const Math::Vector3& velocity)
		{
			mBase->setLinearVelocity(Convert(velocity));
		}

		Math::Vector3 RigidBody::GetVelocity() const
		{
			return ConvertFrom(mBase->getLinearVelocity());
		}

		void RigidBody::SetAngularVelocity(const Math::Vector3& velocity)
		{
			mBase->setAngularVelocity(Convert(velocity));
		}

		Math::Vector3 RigidBody::GetAngularVelocity() const
		{
			return ConvertFrom(mBase->getAngularVelocity());
		}

		void RigidBody::SetDrag(float drag)
		{
			mBase->setLinearDamping(drag);
		}

		float RigidBody::GetDrag() const
		{
			return mBase->getLinearDamping();
		}

		void RigidBody::SetAngularDrag(float drag)
		{
			mBase->setAngularDamping(drag);
		}

		float RigidBody::GetAngularDrag() const
		{
			return mBase->getAngularDamping();
		}

		Math::Vector3 RigidBody::GetInertiaTensor() const
		{
			return ConvertFrom(mBase->getMassSpaceInertiaTensor());
		}

		void RigidBody::SetMaxAngularVelocity(float maxVelocity)
		{
			mBase->setMaxAngularVelocity(maxVelocity);
		}

		float RigidBody::GetMaxAngularVelocity() const
		{
			return mBase->getMaxAngularVelocity();
		}

		Math::Vector3 RigidBody::GetCenterOfMassPosition() const
		{
			PxTransform cMassTfrm = mBase->getCMassLocalPose();
			return ConvertFrom(cMassTfrm.p);
		}

		Math::Quaternion RigidBody::GetCenterOfMassRotation() const
		{
			PxTransform cMassTfrm = mBase->getCMassLocalPose();
			return ConvertFrom(cMassTfrm.q);
		}

		void RigidBody::SetPositionSolverCount(Uint32 count)
		{
			mBase->setSolverIterationCounts(Math::max(1U, count), GetVelocitySolverCount());
		}

		Uint32 RigidBody::GetPositionSolverCount() const
		{
			Uint32 posCount = 1;
			Uint32 velCount = 1;

			mBase->getSolverIterationCounts(posCount, velCount);
			return posCount;
		}

		void RigidBody::SetVelocitySolverCount(Uint32 count)
		{
			mBase->setSolverIterationCounts(GetPositionSolverCount(), Math::max(1U, count));
		}

		Uint32 RigidBody::GetVelocitySolverCount() const
		{
			Uint32 posCount = 1;
			Uint32 velCount = 1;

			mBase->getSolverIterationCounts(posCount, velCount);
			return velCount;
		}
		physx::PxRigidDynamic * RigidBody::GetBase()
		{
			return mBase;
		}
	}
}
#endif