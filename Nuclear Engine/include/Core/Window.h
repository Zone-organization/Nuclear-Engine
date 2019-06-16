#pragma once
#include <Core\ApplicationDesc.h>
#include <Core\Input.h>

struct GLFWwindow;
namespace NuclearEngine
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

		class NEAPI Window
		{
		public:
			static bool Create(Window* window, const WindowDesc& desc);
			static void Destroy(Window* window);

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
