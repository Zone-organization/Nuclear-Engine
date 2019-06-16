#include "Core\Input.h"
#include <Core\Window.h>

#include <GLFW/include/GLFW/glfw3.h>

namespace NuclearEngine
{
	namespace Core
	{



		void GLFW_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{

		}

		void GLFW_KeyCallback(GLFWwindow* window, int key, int, int action, int mods)
		{

		}

		void GLFW_CharCallback(GLFWwindow* window, unsigned int c)
		{

		}

		void GLFW_MouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
		{

		}


		void Input::Initialize(Window * window)
		{
			mParentWindow = window;
			glfwSetCursorPosCallback(window->GetRawWindowPtr(), GLFW_MouseMovementCallback);
			glfwSetScrollCallback(window->GetRawWindowPtr(), GLFW_ScrollCallback);
			glfwSetKeyCallback(window->GetRawWindowPtr(), GLFW_KeyCallback);
			glfwSetCharCallback(window->GetRawWindowPtr(), GLFW_CharCallback);
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
	}
}