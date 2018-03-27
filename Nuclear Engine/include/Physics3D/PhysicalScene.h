#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
namespace physx { class PxScene; }

namespace NuclearEngine
{
	namespace Physics3D
	{

		struct NEAPI PhysicalSceneDesc
		{
			Math::Vector3 gravity = Math::Vector3(0.0f, 0.0f, 0.0f);
		};

		class NEAPI PhysicalScene
		{
		public:
			static bool Create(PhysicalScene* scene,const PhysicalSceneDesc& desc);
			static void Delete(PhysicalScene* scene);

			physx::PxScene* GetBase();
		private:
			physx::PxScene* scene;
		};
	}
}