#pragma once
#include <Base\NE_Common.h>

namespace physx { class PxMaterial; }

namespace NuclearEngine
{
	namespace PhysX
	{
		//Wraps around physx::PxMaterial
		class NEAPI PhysXMaterial
		{
		public:
			PhysXMaterial();
			~PhysXMaterial();

			void Create(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.5f);

			void SetStaticFriction(float value);

			float GetStaticFriction() const;

			void SetDynamicFriction(float value);

			float GetDynamicFriction() const;

			void SetRestitution(float value);

			float GetRestitution() const;

			physx::PxMaterial* GetPtr();
		private:
			physx::PxMaterial* mMaterial;
		};
	}
}