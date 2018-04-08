#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
namespace physx { class PxMaterial; }

namespace NuclearEngine
{
	namespace Physics3D
	{

		class NEAPI PhysicsMaterial
		{
		public:
			static bool Create(PhysicsMaterial* material, float staticFriction, float dynamicFriction, float restitution);
			static void Delete(PhysicsMaterial* material);

			physx::PxMaterial* GetBase();
		private:
			physx::PxMaterial* material;
		};
	}
}