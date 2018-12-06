#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Core\Engine.h>
#include <Engine\Graphics\ImGui_Binding.h>
#pragma comment (lib, "GLAD.lib")

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;
		sf::Window* Application::MainWindow = nullptr;

		bool Application::Start(const ApplicationDesc & Desc)
		{
			// Set report callback to standard output
			LLGL::Log::SetReportCallbackStd();

			// Create render system
			renderer = LLGL::RenderSystem::Load(
				rendererModule_,
#ifdef _DEBUG
				(debugger ? profilerObj_.get() : nullptr),
				(debugger ? debuggerObj_.get() : nullptr)
#else
				nullptr, nullptr
#endif
			);

			// Create render context
			LLGL::RenderContextDescriptor contextDesc;
			{
				contextDesc.videoMode.resolution = resolution;
				contextDesc.vsync.enabled = vsync;
				contextDesc.multiSampling.enabled = (multiSampling > 1);
				contextDesc.multiSampling.samples = multiSampling;

#if defined _DEBUG && 0
				contextDesc.debugCallback = [](const std::string& type, const std::string& message)
				{
					std::cerr << type << ": " << message << std::endl;
				};
#endif

#if defined _WIN32
				contextDesc.profileOpenGL.contextProfile = LLGL::OpenGLContextProfile::CoreProfile;
#elif defined __APPLE__
				contextDesc.profileOpenGL.contextProfile = LLGL::OpenGLContextProfile::CoreProfile;
				contextDesc.profileOpenGL.majorVersion = 4;
				contextDesc.profileOpenGL.minorVersion = 1;
#elif defined __linux__
				/*contextDesc.multiSampling.enabled           = false;
				contextDesc.multiSampling.samples           = 1;*/
				/*contextDesc.profileOpenGL.contextProfile    = LLGL::OpenGLContextProfile::CoreProfile;
				contextDesc.profileOpenGL.majorVersion      = 3;
				contextDesc.profileOpenGL.minorVersion      = 3;*/
#endif
			}
			context = renderer->CreateRenderContext(contextDesc);

			// Create command buffer
			commandsExt = renderer->CreateCommandBufferExt();
			if (!commands)
				commands = renderer->CreateCommandBuffer();

			// Get command queue
			commandQueue = renderer->GetCommandQueue();

			// Initialize command buffer
			commands->SetClearColor(defaultClearColor);
#if 0
			commands->SetRenderTarget(*context);
			commands->SetViewport({ { 0, 0 }, resolution });
			commands->SetScissor({ { 0, 0 }, resolution });
#endif

			// Print renderer information
			const auto& info = renderer->GetRendererInfo();

			std::cout << "renderer information:" << std::endl;
			std::cout << "  renderer:         " << info.rendererName << std::endl;
			std::cout << "  device:           " << info.deviceName << std::endl;
			std::cout << "  vendor:           " << info.vendorName << std::endl;
			std::cout << "  shading language: " << info.shadingLanguageName << std::endl;

			// Set window title
			auto& window = static_cast<LLGL::Window&>(context->GetSurface());

			auto rendererName = renderer->GetName();
			window.SetTitle(title + L" ( " + std::wstring(rendererName.begin(), rendererName.end()) + L" )");

			// Add input event listener to window
			input = std::make_shared<LLGL::Input>();
			window.AddEventListener(input);

			// Change window descriptor to allow resizing
			auto wndDesc = window.GetDesc();
			wndDesc.resizable = true;
			window.SetDesc(wndDesc);

			// Change window behavior
			auto behavior = window.GetBehavior();
			behavior.disableClearOnResize = true;
			behavior.moveAndResizeTimerID = 1;
			window.SetBehavior(behavior);

			// Add window resize listener
			window.AddEventListener(std::make_shared<ResizeEventHandler>(*this, context, projection));

			// Initialize default projection matrix
			projection = PerspectiveProjection(GetAspectRatio(), 0.1f, 100.0f, Gs::Deg2Rad(45.0f));

			// Show window
			window.Show();

			// Store information that loading is done
			loadingDone_ = true;

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
