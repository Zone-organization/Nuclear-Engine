#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine
{
	namespace PhysX
	{
		struct PhysXEngineDesc
		{

		};

		class NEAPI PhysXEngine
		{
		public:
			static bool Initialize(const PhysXEngineDesc& desc);
			static void Shutdown();

		protected:
		};
	}
}