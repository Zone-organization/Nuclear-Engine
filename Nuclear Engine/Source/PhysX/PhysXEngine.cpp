#include <PhysX\PhysXEngine.h>
#include <PhysX\PhysXincluder.h>

#if !defined(PHYSX_NOT_INCLUDED) && defined(NE_USE_PHYSXENGINE)
#include <PhysX\Include\PxPhysicsAPI.h>
using namespace physx;

#pragma comment (lib, "PhysX/Lib/PhysX3_x86.lib")
#pragma comment (lib, "PhysX/Lib/PxFoundation_x86.lib")
#pragma comment (lib, "PhysX/Lib/PxPvdSDK_x86.lib")

namespace NuclearEngine
{
	namespace PhysX
	{
		static PxDefaultAllocator gDefaultAllocatorCallback;
		static PxFoundation* gFoundation;
		static PxPvd* gPVD;
		static PxPhysics* gPhysics;

		class PhysXErrorCallback : public PxErrorCallback
		{
		public:
			virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,	int line)
			{
				Log.Error("[PhysX] " + std::string(message) + " in File: " +  std::string(file) + "  Line : " + std::to_string(line) + "\n");
			}
		};
		static PhysXErrorCallback gErrorCallback;
		bool PhysXEngine::Initialize()
		{
			gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gErrorCallback);
			if (!gFoundation)
			{
				Log.FatalError("[PhysX] Initializing PhysXEngine Failed: PxCreateFoundation Failed...\n");
			}

			bool recordMemoryAllocations = true;

			gPVD = PxCreatePvd(*gFoundation);
			PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			gPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);


			gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), recordMemoryAllocations, gPVD);
			if (!gPhysics)
			{
				Log.FatalError("[PhysX] Initializing PhysXEngine Failed: PxCreatePhysics Failed...\n");
			}
			return false;
		}
		void PhysXEngine::Shutdown()
		{
			gPhysics->release();
			gPVD->release();
			gFoundation->release();
		}
	}
}
#else
namespace NuclearEngine
{
	namespace PhysX
	{

		bool PhysXEngine::Initialize()
		{
			return false;
		}
		void PhysXEngine::Shutdown()
		{
		}
	}
}
#endif