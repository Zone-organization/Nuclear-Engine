#pragma once
#include <NE_Common.h>

namespace physx {
	class PxFoundation;
	class PxPhysics;
	class PxPvd;
	class PxCpuDispatcher;
	class PxMaterial;
	class PxDefaultCpuDispatcher;
}
namespace Nuclear
{
	namespace Components {
		class ColliderComponent;
		class RigidBodyComponent;
	}
	namespace PhysX
	{
		using namespace physx;
		struct PhysXEngineDesc
		{

		};

		class NEAPI PhysXEngine
		{
		public:	
			PhysXEngine(PhysXEngine const&) = delete;
			void operator=(PhysXEngine const&) = delete;

			static PhysXEngine& GetInstance();

			bool Initialize(const PhysXEngineDesc& desc);
			void Shutdown();

			PxFoundation* GetFoundation();
			PxPhysics* GetPhysics();
			PxPvd* GetPvd();
			PxCpuDispatcher* GetCPUDispatcher();

			PxMaterial& GetDefaultMaterial();

		protected:
			PxFoundation* gFoundation = NULL;
			PxPhysics* gPhysics = NULL;
			PxPvd* gPvd = NULL;
			PxDefaultCpuDispatcher* gDispatcher = NULL;
			PxMaterial* gDefaultMaterial;

		private:
			PhysXEngine();
		};
	}
}