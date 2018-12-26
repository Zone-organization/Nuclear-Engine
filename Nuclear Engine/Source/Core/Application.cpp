#include <Core\Application.h>
#include <Core\Engine.h>
#include <Engine\Graphics\ImGuiBinding.h>
#include <Engine\Graphics\Context.h>
#include <GLFW/include/GLFW/glfw3.h>

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;
		GLFWwindow* MainWindow = nullptr;

		bool Application::Start(const ApplicationDesc & Desc)
		{
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			MainWindow = glfwCreateWindow(Desc.WindowWidth, Desc.WindowHeight, Desc.Title.c_str(), NULL, NULL);
			if (MainWindow == NULL)
			{
				Log.FatalError("[Application] Creating Window Failed!\n");
				glfwTerminate();
				return false;
			}

			//Initialize Context
			Graphics::Context::Initialize(Desc);
			
			return true;
		}
		void Application::Shutdown()
		{
			glfwDestroyWindow(MainWindow);
		}
		void Application::Display(bool show)
		{
			if(show)
				glfwShowWindow(MainWindow);
			else
				glfwHideWindow(MainWindow);
		}
	
		bool Application::PollEvents()
		{
			ShouldClose = glfwWindowShouldClose(MainWindow);
			return ShouldClose;
		}

		Float32 Application::GetAspectRatioF32()
		{
			//auto resolution = Graphics::Context::GetContext()->resolution;
			//return (static_cast<float>(resolution.width) / static_cast<float>(resolution.height));
			return 0;
		}

		GLFWwindow * Application::GetWindow()
		{
			return MainWindow;
		}
	
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{

		}
	}
}
