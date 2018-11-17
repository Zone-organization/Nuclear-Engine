#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Engine\Graphics\API\Context.h>
#include <GLAD\include\glad\glad.h>
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/Texture.h>
#include <iostream>
#include <Core\Engine.h>
#include <SFML\Window\ContextType.h>
#pragma comment (lib, "GLAD.lib")

namespace NuclearEngine
{
	namespace Core
	{		
		bool Application::ShouldClose = false;
		sf::Window* Application::MainWindow = nullptr;
		sf::Window* SplashWindow = nullptr;

		void Application::SplashScreen()
		{
			SplashWindow = new sf::Window();
			SplashWindow->create(sf::VideoMode(750, 400), "Nuclear Engine", sf::Style::None);
			std::string VertexShader = R"(struct VertexInputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = float4(input.position,  1);

	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
})";

			std::string PixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.tex);
}
)";
		}

		void APIENTRY glDebugOutput(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar *message,
			const void *userParam)
		{
			// ignore non-significant error/warning codes
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

			std::cout << "\n--------------------------" << std::endl;
			std::cout << "OpenGL Debug message (" << id << "): " << message << std::endl;

			switch (source)
			{
			case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
			} std::cout << std::endl;

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
			} std::cout << std::endl;

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
			} std::cout << std::endl;
			std::cout << "--------------------------" << std::endl;
		}

		bool Application::Start(const ApplicationDesc & Desc)
		{
			Graphics::API::Context::SetRenderAPI(Desc.Renderer);
			sf::SetOpenGLContext(false);

			if (Desc.Renderer == RenderAPI::OpenGL4_5)
				sf::SetOpenGLContext(true);
			
			//Create Window
			MainWindow = new sf::Window();
			sf::ContextSettings settings(24, 8, 0, 4, 5, sf::ContextSettings::Core, false);

			if (Desc.DebugRenderAPI == true)
			{
				settings.attributeFlags = sf::ContextSettings::Debug;
			}
			
			MainWindow->create(sf::VideoMode(Desc.WindowWidth, Desc.WindowHeight), Desc.Title, Desc.Style, settings);
			MainWindow->setVisible(false);

			if (Desc.Renderer == RenderAPI::OpenGL4_5)
			{				
				MainWindow->setActive(true);
				if (Desc.DebugRenderAPI == true)
				{
					glEnable(GL_DEBUG_OUTPUT);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback(glDebugOutput, nullptr);
					glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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
			MainWindow->close();
			delete MainWindow;
		}
		void Application::Display()
		{
			MainWindow->setVisible(true);
		}
		void Application::SwapBuffers()
		{
			if (Graphics::API::Context::GetRenderAPI() == RenderAPI::OpenGL4_5)
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
