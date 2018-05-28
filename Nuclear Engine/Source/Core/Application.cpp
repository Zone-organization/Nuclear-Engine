#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Engine\Graphics\API\Context.h>
#include <GLAD\include\glad\glad.h>
#include "imgui_impl\imgui_impl.h"
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Core\Engine.h>

#pragma comment (lib, "GLAD.lib")

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;
		sf::Window* Application::MainWindow = nullptr;

		bool Application::Start(const ApplicationDesc & Desc)
		{
			Graphics::API::Context::SetRenderAPI(Desc.Renderer);
			MainWindow = new sf::Window();

			MainWindow->create(sf::VideoMode(Desc.WindowWidth, Desc.WindowHeight),Desc.Title,Desc.Style);
					
			if (Desc.Renderer == RenderAPI::OpenGL3)
			{
				MainWindow->setActive(true);
				if (!gladLoadGL())
				{
					Log.Error("[Application] Failed to initialize GLAD in window: " + Desc.Title + "\n");
					return false;
				}

			}

			if (Desc.Renderer == RenderAPI::DirectX11)
			{
				if (!Graphics::API::DirectX::DX11Context::Initialize(MainWindow))
				{
					Log.Error("[Application] Failed to initialize DirectX11 in window: " + Desc.Title + "\n");
					return false;
				}
			}
			
			Log.Info("[Application] Created Application: " + Desc.Title + " Width: " +  std::to_string(MainWindow->getSize().x) + " Height: " + std::to_string(MainWindow->getSize().y) + " \n");
			return true;
		}
		void Application::Shutdown()
		{
			if (Graphics::API::Context::GetRenderAPI() == RenderAPI::DirectX11)
			{
				Graphics::API::DirectX::DX11Context::Shutdown();
			}
			//glfwDestroyWindow(window);
			//glfwTerminate();
		}
		void Application::Display()
		{
			MainWindow->setVisible(true);
		}
		void Application::SwapBuffers()
		{
			if (Graphics::API::Context::GetRenderAPI() == RenderAPI::OpenGL3)
			{
				MainWindow->display();
			}
			else if (Graphics::API::Context::GetRenderAPI() == RenderAPI::DirectX11)
			{
				Graphics::API::DirectX::DX11Context::SwapBuffers();
			}
		}
		bool Application::PollEvents()
		{    
			// handle events
			sf::Event wevent;
			while (MainWindow->pollEvent(wevent))
			{
				if (wevent.type == sf::Event::Closed)
				{
					ShouldClose = false;
				}
				else if (wevent.type == sf::Event::Resized)
				{
				}
			
			}

			return ShouldClose;
		}
		Uint32 Application::GetAspectRatio()
		{
			return (MainWindow->getSize().x / MainWindow->getSize().y);
		}
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{
			/*switch (mode)
			{
			case MouseInputMode::Normal:
				//return //glfwSetInputMode(window, //glfw_CURSOR, //glfw_CURSOR_NORMAL);
			case MouseInputMode::Virtual:
				//return //glfwSetInputMode(window, //glfw_CURSOR, //glfw_CURSOR_DISABLED);
			case MouseInputMode::Hidden:
				//return //glfwSetInputMode(window, //glfw_CURSOR, //glfw_CURSOR_HIDDEN);
			}*/
		}
	}
}
