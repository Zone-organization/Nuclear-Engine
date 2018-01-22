#include <Core\Application.h>
#include <iostream>
namespace NuclearEngine {
	using namespace Platform;
	namespace Core {
		static Clock *windowclock;
#ifdef NUCLEAR_PLATFORM_WINDOWS32
		static Internals::Win32_Window* WinType;
#endif

		bool Application::Create(std::wstring Title, int Width, int Height, bool fullscreen)
		{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
			WinType = new Internals::Win32_Window();
#endif
			windowclock = new Clock();

			if (WinType->Create(Title, Width, Height, fullscreen) == false)
			{
				return false;
			}

			return true;
		}

		void Application::Delete()
		{
			delete WinType;
			delete windowclock;
		}

		void Application::Display()
		{
			WinType->Display();
		}

		void Application::ProcessEvents()
		{
			WinType->ProcessEvents();
		}

		void Application::SetTitle(std::wstring title)
		{
			WinType->SetTitle(title);
		}

		std::wstring Application::GetTitle()
		{
			return WinType->GetTitle();
		}

		int Application::GetWidth()
		{
			return WinType->GetWidth();
		}

		int Application::GetHeight()
		{
			return WinType->GetHeight();
		}

		float Application::GetAspectRatio()
		{
			return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
		}

		bool Application::ShouldClose()
		{
			return WinType->ShouldClose;
		}

		double Application::GetTime()
		{
			return windowclock->GetElapsedTime().AsSeconds();
		}

		Internals::Win32_Window * Application::GetInternalWindow()
		{
			return WinType;
		}
	}
}