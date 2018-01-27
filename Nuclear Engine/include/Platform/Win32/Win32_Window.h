#pragma once
#include <NE_Common.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
#include <string>
#include <Windows.h>

namespace NuclearEngine {
	namespace Platform {
		struct WindowDesc;
		namespace Internals {
			class NEAPI Win32_Window
			{
			public:
				Win32_Window();
				Win32_Window(HANDLE handle);
				~Win32_Window();

				bool Create(const WindowDesc& Desc);
				void Destroy();

				void Display();

				void ProcessEvents();

				void SetSize(uint width, uint height);
				void SetTitle(std::wstring _title);

				bool ShouldClose();

				void GetSize(uint& width, uint& height);
				std::wstring GetTitle();
				uint GetAspectRatioi();			
				float GetAspectRatiof();
				HWND GetHandle();
				HINSTANCE GetInstance();
			private:
				void UpdateCursorImage();
				void UpdateRectClip(bool flag);

				bool active = true;	
				bool shouldClose = false;
				bool fullscreen = false;

				//mouse
				int lastx;
				int lasty;
				int virtualx;
				int virtualy;

				HANDLE WindPropHandle;
				HWND m_Handle = NULL;
				HINSTANCE hInstance;
				WNDCLASS wc;	
				DWORD dwExStyle;
				DWORD dwStyle;
				POINT mousePosition;

				static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
			};
		}
	}
}
#endif