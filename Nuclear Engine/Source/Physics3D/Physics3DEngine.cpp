#include <Physics3D\Physics3DEngine.h>
#include <PhysX\PhysXincluder.h>

#if !defined(PHYSX_NOT_INCLUDED) && defined(NE_USE_PHYSXENGINE)
#include <PhysX\Include\PxPhysicsAPI.h>
using namespace physx;

#pragma comment (lib, "PhysX/Lib/PhysX3_x86.lib")
#pragma comment (lib, "PhysX/Lib/PxFoundation_x86.lib")
#pragma comment (lib, "PhysX/Lib/PxPvdSDK_x86.lib")

namespace NuclearEngine
{
	namespace Physics3D
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
		bool Physics3DEngine::Initialize()
		{
			gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gErrorCallback);
			if (!gFoundation)
			{
				Log.FatalError("[Physics3DEngine] Initializing PhysX Failed: PxCreateFoundation Failed...\n");
				return false;
			}

			bool recordMemoryAllocations = true;

			gPVD = PxCreatePvd(*gFoundation);
			PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			gPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);


			gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), recordMemoryAllocations, gPVD);
			if (!gPhysics)
			{
				Log.FatalError("[Physics3DEngine] Initializing PhysX Failed: PxCreatePhysics Failed...\n");
				return false;
			}

			Log.Info("[Physics3DEngine] Initialized PhysX.\n");
			return true;
		}
		void Physics3DEngine::Shutdown()
		{
			gPhysics->release();
			gPVD->release();
			gFoundation->release();
		}
		physx::PxPhysics * Physics3DEngine::GetContext()
		{
			return gPhysics;
		}
	}
}
#else
namespace NuclearEngine
{
	namespace Physics3D
	{

		bool Physics3DEngine::Initialize()
		{
			return false;
		}
		void Physics3DEngine::Shutdown()
		{
		}
		physx::PxPhysics * Physics3DEngine::GetContext()
		{
			return nullptr;
		}
	}
}
#endif