#pragma once
#include <NuclearPlatform\NP_Common.h>
#include <NuclearPlatform\Clock.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <NuclearPlatform\Win32\Win32_Window.h>
#endif
#include <string>
#include <functional>

namespace NuclearPlatform {
	class NPAPI Application
	{
	public:
		static bool Create(std::wstring Title, int Width, int Height,bool fullscreen);

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

		static void SetMouseMovementCallback(void(*function)(double, double));

		static Internals::Win32_Window* GetInternalWindow();
	};
}