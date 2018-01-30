#pragma once
#include <NE_Common.h>
#ifdef APP_EXPOSE_WIN32
#include <Windows.h>
#endif

struct GLFWwindow;

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual , Normal , Hidden };
		
		enum class NEAPI RenderAPI {
			OpenGL3,
			DirectX11
		};

		struct ApplicationDesc
		{
			uint width = 1024;
			uint height = 768;
			bool fullscreen = false;
			bool resizable = false;
			RenderAPI renderer = RenderAPI::OpenGL3;
			std::string title = "NuclearEngine";
		};

		class NEAPI Application
		{
		public:
			static bool Create(const ApplicationDesc& Desc);
			static void Shutdown();

			static void Display();
			static void Hide();
			static void SwapBuffers();
			static void ProcessEvents();

			static void SetSize(int width, int height);
			static void SetTitle(std::string title);

			static bool ShouldClose();

			static void SetMouseInputMode(const MouseInputMode& mode);

			static void GetSize(int* width, int* height);
			static float GetAspectRatiof();

#ifdef APP_EXPOSE_WIN32
			static HWND GetHandle();
#endif
		};
	}
}
