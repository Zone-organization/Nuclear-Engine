#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Base\Utilities\Accessor.h>
namespace physx { class PxRigidDynamic; }

namespace NuclearEngine
{
	namespace Physics3D
	{
		/*
		A dynamic rigid body.
		TODO: Sometimes in many games theres alot of static stuff try to add RigidBodyStatic as an optimization
		*/
		class NEAPI RigidBody
		{
		public:
			static bool Create(RigidBody* body);
			static void Delete(RigidBody* body);

			void Move(const Math::Vector3& position);
			void Rotate(const Math::Quaternion& rotation);
			Math::Vector3 GetPosition() const;
			Math::Quaternion GetRotation() const;
			void SetTransform(const Math::Vector3& pos, const Math::Quaternion& rot);
			void SetMass(float mass);  //TODO
			float GetMass() const;
			void SetIsKinematic(bool kinematic);
			bool GetIsKinematic() const;
			bool isSleeping() const;
			void Sleep();
			void WakeUp();
			void SetSleepThreshold(float threshold);
			float GetSleepThreshold() const;
			void SetUseGravity(bool gravity);
			bool GetUseGravity() const;
			ACCESSOR_DEC_REF(Math::Vector3, Velocity)


			//void SetVelocity(const Math::Vector3& velocity);
			//Math::Vector3 GetVelocity() const;
			void SetAngularVelocity(const Math::Vector3& velocity);
			Math::Vector3 GetAngularVelocity() const;
			void SetDrag(float drag);
			float GetDrag() const;
			void SetAngularDrag(float drag);
			float GetAngularDrag() const;
			void SetInertiaTensor(const Math::Vector3& tensor);  //TODO
			Math::Vector3 GetInertiaTensor() const;
			void SetMaxAngularVelocity(float maxVelocity);
			float GetMaxAngularVelocity() const;
			void SetCenterOfMass(const Math::Vector3& position, const Math::Quaternion& rotation);  //TODO
			Math::Vector3 GetCenterOfMassPosition() const;
			Math::Quaternion GetCenterOfMassRotation() const;
			void SetPositionSolverCount(Uint32 count);
			Uint32 GetPositionSolverCount() const;
			void SetVelocitySolverCount(Uint32 count);
			Uint32 GetVelocitySolverCount() const;
			
			physx::PxRigidDynamic* GetBase();
		private:
			physx::PxRigidDynamic* mBase;
		};
	}
}