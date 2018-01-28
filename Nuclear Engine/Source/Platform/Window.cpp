#include <Platform\Window.h>
#include <GLAD\include\glad\glad.h>
#include <GLFW\include\glfw3.h>
#include <API\DirectX\DX11Context.h>

namespace NuclearEngine
{
	namespace Platform
	{	
		Window::Window()
		{
		}
		bool Window::InitGLFW()
		{
			glfwInit();

			return true;
		}
		bool Window::Create(const WindowDesc & Desc)
		{
			if (Desc.API != 2)
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
			else {
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			}
			window = glfwCreateWindow(Desc.width, Desc.height, Desc.title.c_str(), NULL, NULL);
			if (window == NULL)
			{
				Log->Error("[Window] Failed to create GLFW window: " + Desc.title + "\n");
				return false;
			}
			if (Desc.API == 2)
			{
				glfwMakeContextCurrent(window);
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					Log->Error("[Window] Failed to initialize GLAD in window: " + Desc.title + "\n");
					return false;
				}
			}

			if (Desc.API == 1)
			{
				if (!API::DirectX::DX11Context::Initialize(window))
				{
					Log->Error("[Window] Failed to initialize DirectX11 in window: " + Desc.title + "\n");
					return false;
				}
			}

			Log->Info("[Window] Created Window: " + Desc.title + "Size: " +  std::to_string(Desc.width) + "," + std::to_string(Desc.height) + " \n");
			return true;
		}
		void Window::Destroy()
		{
			glfwDestroyWindow(window);
		}
		void Window::Display()
		{
			glfwShowWindow(window);
		}
		void Window::Hide()
		{
			glfwHideWindow(window);
		}
		void Window::SwapBuffers()
		{
			if (opengl)
			{
				glfwSwapBuffers(window);
			}
			else {
				API::DirectX::DX11Context::SwapBuffers();
			}
		}
		void Window::ProcessEvents()
		{
			glfwPollEvents();
		}
		void Window::SetSize(int width, int height)
		{
			glfwSetWindowSize(window, width, height);
		}
		void Window::SetTitle(std::string title)
		{
			glfwSetWindowTitle(window, title.c_str());
		}
		bool Window::ShouldClose()
		{
			return glfwWindowShouldClose(window);
		}
		void Window::GetSize(int* width, int* height)
		{
			glfwGetWindowSize(window,width, height);
		}
	}
}
