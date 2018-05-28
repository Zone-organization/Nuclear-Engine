#pragma once
#include <Base\NE_Common.h>
#include <ThirdParty\SFML\Window.hpp>

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual , Normal , Hidden };
		
		enum class NEAPI RenderAPI : Uint8
		{
			OpenGL3,
			DirectX11
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			Uint32 Style = sf::Style::Default;
			RenderAPI Renderer = RenderAPI::OpenGL3;
			std::string Title = "NuclearEngine";
		};

		class NEAPI Application
		{
		public:
			static bool Start(const ApplicationDesc& Desc);
			static void Shutdown();

			static void Display();

			static void SwapBuffers();

			static bool ShouldClose;

			static bool PollEvents();
			static void SetMouseInputMode(const MouseInputMode& mode);

			static Uint32 GetAspectRatio();
			static sf::Window* MainWindow;
		};
	}
}
