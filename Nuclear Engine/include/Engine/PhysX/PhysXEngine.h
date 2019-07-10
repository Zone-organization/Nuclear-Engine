#pragma once
#include <Base\NE_Common.h>
#include <PhysX/include/PxPhysicsAPI.h>

namespace NuclearEngine
{
	namespace ECS { class Scene; }

	namespace PhysX
	{
		using namespace physx;
		struct PhysXEngineDesc
		{

		};

		class NEAPI PhysXEngine
		{
		public:
			static bool Initialize(const PhysXEngineDesc& desc);
			static void Shutdown();

			static PxFoundation* GetFoundation();
			static PxPhysics* GetPhysics();
			static PxPvd* GetPvd();
			static PxCpuDispatcher* GetCPUDispatcher();


			//Factory
		};
	}
}