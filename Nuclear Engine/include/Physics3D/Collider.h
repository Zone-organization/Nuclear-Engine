#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Physics3D\RigidBody.h>
#include <Physics3D\PhysicsMaterial.h>

namespace physx { class PxShape; }

namespace NuclearEngine
{
	namespace Physics3D
	{

		class NEAPI Collider
		{
		public:
			static bool Create(Collider* material);
			static void Delete(Collider* material);

			Math::Vector3 GetPosition() const;
			Math::Quaternion GetRotation() const;
			void SetTransform(const Math::Vector3& pos, const Math::Quaternion& rot);
	
			void SetIsTrigger(bool value);
			bool GetIsTrigger() const;
			void SetRigidBody(RigidBody* value);
			RigidBody* GetRigidBody();

			void SetMaterial(const PhysicsMaterial& material);
			PhysicsMaterial GetMaterial() const;

			physx::PxShape* GetBase();
		private:
			physx::PxShape* mBase;
			RigidBody* mRigidBody = nullptr;
			PhysicsMaterial mMaterial;
			bool isTrigger = false;
		};
	}
}