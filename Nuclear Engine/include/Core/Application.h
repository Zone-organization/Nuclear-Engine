#pragma once
#include <Core\ApplicationDesc.h>

namespace NuclearEngine {
	namespace Core {
		class Window;
		class NEAPI Application
		{
		public:
			static bool Start(const ApplicationDesc& Desc);
			static void Shutdown();
			
			static bool PollEvents();

			static Window* GetMainWindow();
		};
	}
}
