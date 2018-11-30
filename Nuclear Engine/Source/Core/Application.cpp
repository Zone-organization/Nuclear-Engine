#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Engine\Graphics\API\Context.h>
#include <Core\Engine.h>
#include <SFML\Window\ContextType.h>
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
			sf::SetOpenGLContext(false);

			if (Desc.Renderer == RenderAPI::OpenGL4_5)
			{	sf::SetOpenGLContext(true); }
			
			//Create Window
			MainWindow = new sf::Window();
			sf::ContextSettings settings(24, 8, 0, 4, 5, sf::ContextSettings::Core, false);

			if (Desc.DebugRenderAPI == true) { settings.attributeFlags = sf::ContextSettings::Debug; }
			
			MainWindow->create(sf::VideoMode(Desc.WindowWidth, Desc.WindowHeight), Desc.Title, Desc.Style, settings);
			MainWindow->setVisible(false);

			bool result;
			result = Graphics::API::Context::Initialize(MainWindow);

			Log.Info("[Application] Created Application: " + Desc.Title + " Width: " +  std::to_string(MainWindow->getSize().x) + " Height: " + std::to_string(MainWindow->getSize().y) + " \n");
			
			return result;
		}
		void Application::Shutdown()
		{			
			MainWindow->close();
			delete MainWindow;
		}
		void Application::Display()
		{
			MainWindow->setVisible(true);
		}
		void Application::SwapBuffers()
		{
			Graphics::API::Context::SwapBuffers();
		}
		bool Application::PollEvents()
		{    
			// handle events
			sf::Event wevent;
			while (MainWindow->pollEvent(wevent))
			{
				if (wevent.type == sf::Event::Closed)
				{
					ShouldClose = true;
				}
				else if (wevent.type == sf::Event::Resized)
				{
					Engine::GetGame()->OnWindowResize(wevent.size.width, wevent.size.height);
				}
				else if (wevent.type == sf::Event::MouseMoved)
				{
					Engine::GetGame()->OnMouseMovement(wevent.mouseMove.x, wevent.mouseMove.y);
				}
			}

			return ShouldClose;
		}
		Uint32 Application::GetAspectRatio()
		{
			return (MainWindow->getSize().x / MainWindow->getSize().y);
		}
		Float32 Application::GetAspectRatioF32()
		{
			return static_cast<Float32>(MainWindow->getSize().x) / static_cast<Float32>(MainWindow->getSize().y);
		}
	
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{

		}
	}
}
