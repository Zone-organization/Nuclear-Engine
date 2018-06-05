#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Engine\Graphics\API\Context.h>
#include <GLAD\include\glad\glad.h>
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/Texture.h>

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

		bool Application::Start(const ApplicationDesc & Desc)
		{
			Graphics::API::Context::SetRenderAPI(Desc.Renderer);
			sf::SetOpenGLContext(false);

			if (Desc.Renderer == RenderAPI::OpenGL3)
				sf::SetOpenGLContext(true);
			
			//Create Window
			MainWindow = new sf::Window();
			sf::ContextSettings settings(24, 8, 0, 3, 3,	sf::ContextSettings::Core, false);


			MainWindow->create(sf::VideoMode(Desc.WindowWidth, Desc.WindowHeight), Desc.Title, Desc.Style, settings);
			MainWindow->setVisible(false);

			if (Desc.Renderer == RenderAPI::OpenGL3)
			{
				MainWindow->setActive(true);
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
