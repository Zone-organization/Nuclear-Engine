#include "Engine\PhysX\PhysXEngine.h"

using namespace physx;
PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")
namespace NuclearEngine
{
	namespace PhysX
	{
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
	}
}