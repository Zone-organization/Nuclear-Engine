#pragma once
#include <Core\ApplicationDesc.h>
#include <Core\Window.h>

namespace NuclearEngine {
	namespace Core {

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
