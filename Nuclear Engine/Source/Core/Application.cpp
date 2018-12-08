#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Core\Engine.h>
#include <Engine\Graphics\ImGui_Binding.h>
#include <Engine\Graphics\Context.h>
#pragma comment(lib,"LLGL.lib")

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;
		LLGL::Window* MainWindow = nullptr;

		bool Application::Start(const ApplicationDesc & Desc)
		{
			//Initialize Context
			Graphics::Context::Initialize(Desc);
			MainWindow = &static_cast<LLGL::Window&>(Graphics::Context::GetRenderContext()->GetSurface());

			MainWindow->SetTitle(Desc.Title);

			return true;
		}
		void Application::Shutdown()
		{
			//MainWindow->
			//MainWindow->close();
			//delete MainWindow;
		}
		void Application::Display(bool show)
		{
			MainWindow->Show(show);
		}
	
		bool Application::PollEvents()
		{    
			ShouldClose = MainWindow->ProcessEvents();

			return ShouldClose;
		}

		Float32 Application::GetAspectRatioF32()
		{
			auto resolution = Graphics::Context::GetRenderContext()->GetVideoMode().resolution;
			return (static_cast<float>(resolution.width) / static_cast<float>(resolution.height));
		}

		LLGL::Window * Application::GetWindow()
		{
			return MainWindow;
		}
	
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{

		}
	}
}
