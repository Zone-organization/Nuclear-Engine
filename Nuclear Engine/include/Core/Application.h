#pragma once
#include <Core\ApplicationDesc.h>
#include <SFML\Window.hpp>

namespace NuclearEngine {
	namespace Core {

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
			static Float32 GetAspectRatioF32();
			static sf::Window* MainWindow;
		private:
			static void SplashScreen();
		};
	}
}
