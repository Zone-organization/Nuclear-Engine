#include "Engine\PhysX\PhysXEngine.h"
#include "Engine\Components\ColliderComponent.h"
#include <PhysX/include/PxPhysicsAPI.h>
#include <Core\Logger.h>

using namespace physx;
PxDefaultAllocator		gAllocator;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;

#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXCommon_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")

#define PX_RELEASE(x) if(x) { x->release(); }

namespace Nuclear
{
	namespace PhysX
	{
		class NuclearPXErrorCallback : public PxErrorCallback
		{
		public:
			virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override
			{
				NUCLEAR_ERROR("[PhysX] Error Code: '{0}' Details: '{1}'", code , message);
			}
		}gErrorCallback;

		PxMaterial* gDefaultMaterial;

		bool PhysXEngine::Initialize(const PhysXEngineDesc& desc)
		{
			gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
			
			gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
			PxInitExtensions(*gPhysics, nullptr);

			PxU32 numCores = 4;
			gDispatcher = PxDefaultCpuDispatcherCreate(numCores == 0 ? 0 : numCores - 1);
			
			// setup default material...
			gDefaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
			if (!gDefaultMaterial)
			{
				NUCLEAR_FATAL("[PhysXEngine] createMaterial failed");
			}
			NUCLEAR_INFO("[PhysXEngine] PhysX has been initalized succesfully!");
			return true;
		}
		void PhysXEngine::Shutdown()
		{
			PX_RELEASE(gDispatcher)
			PX_RELEASE(gPhysics)
			PX_RELEASE(gFoundation)	
		}
		PxFoundation* PhysXEngine::GetFoundation()
		{
			return gFoundation;
		}
		PxPhysics* PhysXEngine::GetPhysics()
		{
			return gPhysics;
		}
		PxPvd* PhysXEngine::GetPvd()
		{
			return gPvd;
		}
		PxCpuDispatcher* PhysXEngine::GetCPUDispatcher()
		{
			return gDispatcher;
		}
		PxMaterial& PhysXEngine::GetDefaultMaterial()
		{
			return *gDefaultMaterial;
		}
	}
}