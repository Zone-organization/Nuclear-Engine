#pragma once
#include <Core\Input.h>
#include <string>

struct SDL_Window;
struct SDL_Surface;
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

			bool Create(const WindowDesc& desc);
			void Destroy();

			void Display(bool show = true);

			void GetSize(Uint32& width, Uint32& height);
			void GetSize(int& width, int& height);
			Float32 GetAspectRatioF32();

			void SetSize(Uint32 width, Uint32 height);
			void SetTitle(const std::string& title);

			void Update();

			KeyboardKeyStatus GetKeyStatus(KeyboardKey key) override;

			void SetMouseInputMode(const MouseInputMode& mode) override;

			Uint32 GetMouseState(int* xpos, int* ypos) override;
			const Uint8* GetKeyboardState() override;
			SDL_Window* GetSDLWindowPtr();
		private:
			const Uint8* pKeyboardStateArray;
			SDL_Window* pWindow;
			SDL_Surface* pSurface;
		};
	}
}
