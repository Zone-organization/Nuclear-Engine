#pragma once
#include <NE_Common.h>

namespace NuclearEngine
{
	namespace Physics3D
	{
		//Nvidia PhysX is for 3D physics only!!
		class NEAPI Physics3DEngine
		{
		public:
			static bool Initialize();
			static void Shutdown();
		};
	}
}