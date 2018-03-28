#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
namespace physx { class PxScene; }

namespace NuclearEngine
{
	namespace Physics3D
	{

		struct NEAPI PhysicsSceneDesc
		{
			Math::Vector3 gravity = Math::Vector3(0.0f, 0.0f, 0.0f);
		};

		class NEAPI PhysicsScene
		{
		public:
			static bool Create(PhysicsScene* scene,const PhysicsSceneDesc& desc);
			static void Delete(PhysicsScene* scene);

			physx::PxScene* GetBase();
		private:
			physx::PxScene* scene;
		};
	}
}