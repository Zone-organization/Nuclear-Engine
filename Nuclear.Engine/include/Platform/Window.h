#pragma once
#include <NE_Common.h>
#include <string>

struct SDL_Window;
struct SDL_Surface;
namespace Nuclear
{
	namespace Platform
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
			Window();
			~Window();

			bool Create(const WindowDesc& desc);
			void Destroy();

			void Display(bool show = true);

			void GetSize(Uint32& width, Uint32& height);
			void GetSize(int& width, int& height);
			Float32 GetAspectRatioF32();

			void SetSize(Uint32 width, Uint32 height);
			void SetTitle(const std::string& title);

			SDL_Window* GetSDLWindowPtr();
		private:
			SDL_Window* pWindow;
		};
	}
}
