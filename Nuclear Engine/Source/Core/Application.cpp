#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Core\Engine.h>
#include <Engine\Graphics\ImGui_Binding.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;

		bool Application::Start(const ApplicationDesc & Desc)
		{
			LLGL::WindowDescriptor windowDesc;
			windowDesc.title = Desc.Title;
			windowDesc.visible = true;
			windowDesc.centered = true;
			windowDesc.size.width = Desc.WindowWidth;
			windowDesc.size.height = Desc.WindowHeight;
			
			MainWindow = LLGL::Window::Create(windowDesc);
		

			//return result;
		}
		void Application::Shutdown()
		{			
			//MainWindow->close();
			//delete MainWindow;
		}
		void Application::Display()
		{
			///MainWindow->setVisible(true);
		}
		void Application::SwapBuffers()
		{
			//LLGL::Context::SwapBuffers();
		}
		bool Application::PollEvents()
		{    
			// handle events
		/*	sf::Event wevent;
			while (MainWindow->pollEvent(wevent))
			{
				Graphics::ImGui_Binding::ProcessEvent(wevent);

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
			}*/

			return ShouldClose;
		}

		Float32 Application::GetAspectRatioF32()
		{
			auto resolution = Graphics::Context::mContext->GetVideoMode().resolution;
			return (static_cast<float>(resolution.width) / static_cast<float>(resolution.height));
		}
	
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{

		}
	}
}
