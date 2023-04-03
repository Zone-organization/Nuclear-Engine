#include <Platform/Input.h>
#include <SDL\include\SDL.h>

namespace Nuclear
{
	namespace Platform
	{
		Input& Input::Get()
		{
			static Input gInput;
			return gInput;
		}
		const Uint8* Input::GetKeyboardState()
		{
			return pKeyboardStateArray;
		}
		bool Input::IsKeyPressed(KeyCode key)
		{
			if (pKeyboardStateArray[key])
				return true;
			else
				return false;
		}
		void Input::SetMouseInputMode(const MouseInputMode& mode)
		{
			switch (mode)
			{
			case MouseInputMode::Normal:
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);

				return;
			case MouseInputMode::Locked:
				SDL_SetRelativeMouseMode(SDL_TRUE);
				SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
				return;
			case MouseInputMode::Hidden:
				SDL_SetRelativeMouseMode(SDL_FALSE);
				SDL_ShowCursor(SDL_DISABLE);

				return;
			}
		}
		Uint32 Input::GetMouseState(int* xpos, int* ypos)
		{
			return SDL_GetMouseState(xpos, ypos);
		}
		void Input::Update()
		{
			pKeyboardStateArray = SDL_GetKeyboardState(NULL);
		}
		Input::Input()
		{

		}
	}
}