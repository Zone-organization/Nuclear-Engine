#include "Core\Window.h"
#include <GLFW/include/GLFW/glfw3.h>
#include <Core\Logger.h>

#pragma comment(lib,"glfw3.lib")

namespace Nuclear
{
	namespace Core 
	{
		

		bool Window::Create(const WindowDesc & Desc)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			mWindow = glfwCreateWindow(Desc.WindowWidth, Desc.WindowHeight, Desc.Title.c_str(), NULL, NULL);
			if (mWindow == NULL)
			{
				Log.Error("[Window] Creating Window  \""  + Desc.Title + "\" Failed!\n");
				return false;
			}

			mInput.Initialize(this);

			return true;
		}
		void Window::Destroy()
		{
			glfwDestroyWindow(mWindow);
		}


		void Window::Display(bool show)
		{
			if (show)
				glfwShowWindow(mWindow);
			else
				glfwHideWindow(mWindow);
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
			glfwGetWindowSize(mWindow, &width, &height);
		}
		Float32 Window::GetAspectRatioF32()
		{
			int width = 800, height = 600;
			GetSize(width, height);
			return static_cast<float>(width) / static_cast<float>(height);
		}

		void Window::SetSize(Uint32 width, Uint32 height)
		{
			glfwSetWindowSize(mWindow, width, height);
		}
		void Window::SetTitle(const std::string& title)
		{
			glfwSetWindowTitle(mWindow, title.c_str());
		}

		bool Window::ShouldClose()
		{
			return glfwWindowShouldClose(mWindow);
		}
		bool Window::InitializeGLFW()
		{
			return glfwInit();
		}
		void Window::ShutdownGLFW()
		{
			return glfwTerminate();
		}
		void Window::PollEventsGLFW()
		{
			return glfwPollEvents();
		}

		Input * Window::GetInput()
		{
			return &mInput;
		}
		GLFWwindow * Window::GetRawWindowPtr()
		{
			return mWindow;
		}
	}
}