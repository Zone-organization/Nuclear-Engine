#include "Platform\Window.h"
#include <SDL\include\SDL.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Platform 
	{
		bool Window::Create(const WindowDesc & Desc)
		{
			
			pWindow = SDL_CreateWindow(Desc.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Desc.WindowWidth, Desc.WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			if (pWindow == NULL)
			{
				NUCLEAR_ERROR("[Window] Creating Window {0} Failed! SDL_ERROR: {1}", Desc.Title, SDL_GetError());
				return false;
			}
			pSurface = SDL_GetWindowSurface(pWindow);

			return true;
		}
		void Window::Destroy()
		{
			SDL_DestroyWindow(pWindow);
		}

		void Window::Display(bool show)
		{
			if (show)
				SDL_ShowWindow(pWindow);
			else
				SDL_HideWindow(pWindow);
		}


		void Window::GetSize(Uint32& awidth, Uint32& aheight)
		{
			int width, height;

			GetSize(width, height);
			awidth = static_cast<Uint32>(width);
			aheight = static_cast<Uint32>(height);
		}
		void Window::GetSize(int& width, int& height)
		{
			SDL_GetWindowSize(pWindow, &width, &height);
		}

		Float32 Window::GetAspectRatioF32()
		{
			int width = 800, height = 600;
			GetSize(width, height);
			return  static_cast<float>(width) / static_cast<float>(height);
		}

		void Window::SetSize(Uint32 width, Uint32 height)
		{
			SDL_SetWindowSize(pWindow, width, height);
		}
		void Window::SetTitle(const std::string& title)
		{
			SDL_SetWindowTitle(pWindow, title.c_str());
		}

		void Window::Update()
		{
			pKeyboardStateArray = SDL_GetKeyboardState(NULL);
		}
	
		Window::KeyboardKeyStatus Window::GetKeyStatus(KeyboardKey key)
		{
			if (pKeyboardStateArray[key])
				return Window::KeyboardKeyStatus::Pressed;
			else
				return Window::KeyboardKeyStatus::Released;
		}

		void Window::SetMouseInputMode(const MouseInputMode& mode)
		{			
			switch (mode)
			{
			case MouseInputMode::Normal:
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);

				return;
			case MouseInputMode::Virtual:
				SDL_SetRelativeMouseMode(SDL_TRUE);

				return;
			case MouseInputMode::Hidden:
				SDL_SetRelativeMouseMode(SDL_FALSE);
				SDL_ShowCursor(SDL_DISABLE);

				return;
			}
		}
		Uint32 Window::GetMouseState(int* xpos, int* ypos)
		{
			return SDL_GetMouseState(xpos, ypos);
		}
		const Uint8* Window::GetKeyboardState()
		{
			return pKeyboardStateArray;
		}

		SDL_Window* Window::GetSDLWindowPtr()
		{
			return pWindow;
		}
	}
}