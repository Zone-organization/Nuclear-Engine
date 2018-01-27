#pragma once
#include <NE_Common.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
#include <Platform\Win32\Win32_Window.h>
#endif
namespace NuclearEngine {
	namespace Platform {
		struct WindowDesc
		{
			uint width = 1024;
			uint height = 768;
			bool fullscreen =false;
			bool resizable = false;
			std::wstring title = L"NuclearEngine";
		};
		class NEAPI Window
#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
			: public Internals::Win32_Window
#endif
		{
		public:
			Window();
		};

	}
}
