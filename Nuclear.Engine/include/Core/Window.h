#pragma once
#include <Core\Input.h>
#include <string>

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

		class NEAPI Window : public Input
		{
		public:
			static bool InitializeGLFW();
			static void ShutdownGLFW();
			void PollEvents();

			bool Create(const WindowDesc& desc);
			void Destroy();

			void Display(bool show = true);

			void GetSize(Uint32& width, Uint32& height);
			void GetSize(int& width, int& height);
			Float32 GetAspectRatioF32();

			void UpdateSize();
			void SetSize(Uint32 width, Uint32 height);
			void SetTitle(const std::string& title);

			bool ShouldClose();

			KeyboardKeyStatus GetKeyStatus(KeyboardKey key) override;

			void SetMouseInputMode(const MouseInputMode& mode) override;

			void GetMousePosition(double* xpos, double* ypos) override;

			GLFWwindow* GetRawWindowPtr();
		private:
			GLFWwindow* mWindow;
			int mWidth, mHeight;
		};
	}
}
