#include <Core\Application.h>
#include <Core\Engine.h>
#include <Engine\Graphics\Context.h>
#include <GLFW/include/GLFW/glfw3.h>

namespace NuclearEngine
{
	namespace Core
	{		
		Window MainWindow;
		void ResizeCallback(GLFWwindow* window, int Width, int Height)
		{
			Core::Engine::GetGame()->OnWindowResize(Width, Height);
		}


		bool Application::Start(const ApplicationDesc & Desc)
		{
			glfwInit();

			if(!Window::Create(&MainWindow, { Desc.WindowWidth, Desc.WindowHeight, Desc.FullScreen, Desc.Title }))
			{
				Log.FatalError("[Application] Creating Main Window Failed!\n");
				glfwTerminate();
				return false;
			}
			glfwSetFramebufferSizeCallback(MainWindow.GetRawWindowPtr(), ResizeCallback);

			return true;
		}
		void Application::Shutdown()
		{
			Window::Destroy(&MainWindow);
			Graphics::Context::ShutDown();
		}

		bool Application::PollEvents()
		{
			glfwPollEvents();
			return MainWindow.ShouldClose();
		}

		Window * Application::GetMainWindow()
		{
			return &MainWindow;
		}
	}
}
