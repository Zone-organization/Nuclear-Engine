#include "Engine\PhysX\PhysXEngine.h"
#include "Engine\Components\ColliderComponent.h"
#include <PhysX/include/PxPhysicsAPI.h>

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

namespace NuclearEngine
{
	namespace PhysX
	{
		class NuclearPXErrorCallback : public PxErrorCallback
		{
		public:
			virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override
			{
				Log.Error("[PhysX] Error Code: " + std::to_string(code) + " Details: " + std::string(message) + "\n");
			}
		}gErrorCallback;


		bool PhysXEngine::Initialize(const PhysXEngineDesc& desc)
		{
			gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
			
			gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
			PxInitExtensions(*gPhysics, nullptr);

			PxU32 numCores = 4;
			gDispatcher = PxDefaultCpuDispatcherCreate(numCores == 0 ? 0 : numCores - 1);
			

			Log.Info("[PhysXEngine] PhysX has been initalized succesfully!\n");
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
	}
}