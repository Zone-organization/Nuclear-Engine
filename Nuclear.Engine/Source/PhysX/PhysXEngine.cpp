#include "PhysX\PhysXEngine.h"
#include "Components\ColliderComponent.h"
#include <PhysX/include/PxPhysicsAPI.h>
#include <Utilities/Logger.h>


#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXCommon_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")

#define PX_RELEASE(x) if(x) { x->release(); }
using namespace physx;

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

		PhysXEngine::PhysXEngine()
		{

		}
		PhysXEngine& PhysXEngine::GetInstance()
		{
			static PhysXEngine pxengine;

			return pxengine;
		}
		PxDefaultAllocator gAllocator;

		bool PhysXEngine::Initialize(const PhysXEngineDesc& desc)
		{
			gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
			
			gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);

			if (gPhysics == NULL)
			{
				NUCLEAR_ERROR("[PhysXEngine] PxCreatePhysics Failed!");
				return false;
			}

			PxInitExtensions(*gPhysics, nullptr);

			PxU32 numCores = 4;
			gDispatcher = PxDefaultCpuDispatcherCreate(numCores == 0 ? 0 : numCores - 1);

			if (!gDispatcher)
			{
				NUCLEAR_ERROR("[PhysXEngine] PxDefaultCpuDispatcherCreate Failed!");
				return false;
			}

			// setup default material...
			gDefaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
			if (!gDefaultMaterial)
			{
				NUCLEAR_ERROR("[PhysXEngine] createMaterial(gDefaultMaterial) Failed!");
				return false;
			}

			NUCLEAR_INFO("[PhysXEngine] PhysX has been initialized succesfully!");
			return true;
		}
		void PhysXEngine::Shutdown()
		{
			NUCLEAR_INFO("[PhysXEngine] Shutting down...");

			PX_RELEASE(gDefaultMaterial)
			PX_RELEASE(gPvd)
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