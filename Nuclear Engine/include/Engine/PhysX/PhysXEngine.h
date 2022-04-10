#pragma once
#include <Core\NE_Common.h>

namespace physx {
	class PxFoundation;
	class PxPhysics;
	class PxPvd;
	class PxCpuDispatcher;
	class PxMaterial;
}
namespace NuclearEngine
{
	namespace ECS { class Scene; }
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
			static bool Initialize(const PhysXEngineDesc& desc);
			static void Shutdown();

			static PxFoundation* GetFoundation();
			static PxPhysics* GetPhysics();
			static PxPvd* GetPvd();
			static PxCpuDispatcher* GetCPUDispatcher();

			static PxMaterial& GetDefaultMaterial();
		};
	}
}