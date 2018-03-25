#pragma once
#include <NE_Common.h>


//TODO: Move to buildConfig.h
#define NE_COMPILE_PHYSX

namespace NuclearEngine
{
	namespace PhysX
	{
		//Nvidia PhysX is for 3D physics only!!
		class NEAPI PhysXEngine
		{
		public:
			static bool Initialize();
			static void Shutdown();
		};
	}
}