#pragma once
#include <NE_Common.h>
#include <Platform\Clock.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <Platform\Win32\Win32_Window.h>
#endif
#include <string>
namespace NuclearEngine {
	namespace Core {

		class NEAPI Application
		{
		public:
			static bool Create(std::wstring Title, int Width, int Height, bool fullscreen);

			static void Delete();

			static void Display();

			static void ProcessEvents();

			static void SetTitle(std::wstring title);

			static std::wstring GetTitle();

			static int GetWidth();

			static int GetHeight();

			static float GetAspectRatio();

			static bool ShouldClose();

			static double GetTime();

#ifdef NUCLEAR_PLATFORM_WINDOWS32
			static Platform::Internals::Win32_Window* GetInternalWindow();
#endif

		};
	}
}