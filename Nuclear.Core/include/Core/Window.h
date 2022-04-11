#pragma once
#include <Core\ApplicationDesc.h>
#include <Core\Input.h>

struct GLFWwindow;
namespace Nuclear
{
	namespace Core 
	{

		struct WindowDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			bool FullScreen = false;
			std::string Title = "NuclearEngine";
		};

		class Window
		{
		public:
			static bool InitializeGLFW();
			static void ShutdownGLFW();
			static void PollEventsGLFW();

			bool Create(const WindowDesc& desc);
			void Destroy();

			void Display(bool show = true);

			void GetSize(Uint32& width, Uint32& height);
			void GetSize(int& width, int& height);
			Float32 GetAspectRatioF32();

			void SetSize(Uint32 width, Uint32 height);
			void SetTitle(const std::string& title);

			bool ShouldClose();

			Input* GetInput();
			GLFWwindow* GetRawWindowPtr();
		private:
			GLFWwindow* mWindow;
			Input mInput;
		};
	}
}
