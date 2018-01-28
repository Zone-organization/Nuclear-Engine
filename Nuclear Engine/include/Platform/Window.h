#pragma once
#include <NE_Common.h>

struct GLFWwindow;

namespace NuclearEngine {
	namespace Platform {
		enum class MouseInputMode { Virtual , Normal , Hidden };


		struct WindowDesc
		{
			uint width = 1024;
			uint height = 768;
			bool fullscreen = false;
			bool resizable = false;
			// 0 =none , 1 = directx11, 2 = opengl
			int API = 0;
			std::string title = "NuclearEngine";
		};
		class NEAPI Window
		{
		public:

			Window();

			static bool InitGLFW();
			bool Create(const WindowDesc& Desc);
			void Destroy();

			void Display();
			void Hide();
			void SwapBuffers();
			void ProcessEvents();

			void SetSize(int width, int height);
			void SetTitle(std::string title);

			bool ShouldClose();

			//void SetMousePosition(int x, int y);
			//void GetMousePosition(int& x, int& y);
			//void SetMouseInputMode(Input::Mouse::InputMode mode);

			void GetSize(int* width, int* height);
			
		private:
			bool opengl;
			GLFWwindow * window;
		};
	}
}
