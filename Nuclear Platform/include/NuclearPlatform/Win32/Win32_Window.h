#pragma once
#include <NuclearPlatform\NP_Common.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <string>
#include <Windows.h>

namespace NuclearPlatform {

	namespace Internals {

		class NPAPI Win32_Window
		{
		public:
			Win32_Window();
			Win32_Window(HANDLE handle);
			~Win32_Window();

			bool Create(std::wstring title, int width, int height, bool fullscreenflag);

			void Display();

			void ProcessEvents();

			void KillWindow();

			bool ShouldClose = false;

			unsigned int GetWidth();

			unsigned int GetHeight();

			const wchar_t* GetTitle();

			void updateCursorImage();

			void SetTitle(std::wstring _title);

			void UpdateRectClip(bool flag);

			void SetMouseMovementCallback(void(*MVCB)(double, double));

			HWND GetHandle();

			HINSTANCE GetInstance();		// Holds The Instance Of The Application

		private:
			BOOL InitWindow();

			BOOL CleanWindow();

			HANDLE WindPropHandle;

			HWND m_Handle = NULL;		// Holds Our Window Handle

			unsigned int Width;
			unsigned int Height;

			LPCWSTR Title;

			HINSTANCE	hInstance;		// Holds The Instance Of The Application

			bool	active = TRUE;		// Window Active Flag Set To TRUE By Default

			bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

			WNDCLASS	wc;						// Windows Class Structure
			DWORD		dwExStyle;				// Window Extended Style
			DWORD		dwStyle;				// Window Style
			RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
			POINT mousePosition;


			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		};
	}
}
#endif