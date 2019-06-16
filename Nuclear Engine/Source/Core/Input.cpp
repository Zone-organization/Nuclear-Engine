#include "Core\Input.h"
#include <Core\Window.h>

#include <GLFW/include/GLFW/glfw3.h>

namespace NuclearEngine
{
	namespace Core
	{

		void Input::Initialize(Window * window)
		{
			mParentWindow = window;
		}

		Input::KeyboardKeyStatus Input::GetKeyStatus(Input::KeyboardKey key)
		{
			return static_cast<KeyboardKeyStatus>(glfwGetKey(mParentWindow->GetRawWindowPtr(), static_cast<int>(key)));
		}
		void Input::SetMouseInputMode(const MouseInputMode & mode)
		{
			switch (mode)
			{
			case MouseInputMode::Normal:
				return glfwSetInputMode(mParentWindow->GetRawWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			case MouseInputMode::Virtual:
				return glfwSetInputMode(mParentWindow->GetRawWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			case MouseInputMode::Hidden:
				return glfwSetInputMode(mParentWindow->GetRawWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}
		void Input::GetMousePosition(double* xpos, double* ypos)
		{
			glfwGetCursorPos(mParentWindow->GetRawWindowPtr(), xpos, ypos);
		}
	}
}