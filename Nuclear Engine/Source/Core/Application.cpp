#define APP_EXPOSE_WIN32
#include <Core\Application.h>
#include <Core\Context.h>
#include <GLAD\include\glad\glad.h>
#include <GLFW\include\GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\include\GLFW\glfw3native.h>
#include "imgui_impl\imgui_impl.h"

#include <API\DirectX\DX11Context.h>
#include <Core\Engine.h>

#pragma comment (lib, "GLAD.lib")
#pragma comment (lib, "glfw3.lib")

namespace NuclearEngine
{
	namespace Core
	{	
		static GLFWwindow* window;

		void window_size_callback(GLFWwindow* window, int width, int height)
		{
			Engine::GetGame()->OnWindowResize(width, height);
		}
		void mouse_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Engine::GetGame()->OnMouseMovement(xpos, ypos);
		}
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			Imgui_Scroll_Callback(yoffset);
			Engine::GetGame()->OnMouseScroll(xoffset, yoffset);
		}

		void mouse_button_callback(GLFWwindow*, int button, int action, int mods)
		{
			Imgui_mouse_button_callback(button, action);
		}
		void key_callback(GLFWwindow*, int key, int, int action, int mods)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (action == GLFW_PRESS)
				io.KeysDown[key] = true;
			if (action == GLFW_RELEASE)
				io.KeysDown[key] = false;

			(void)mods; // Modifiers are not reliable across systems
			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		}

		void char_callback(GLFWwindow*, unsigned int c)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (c > 0 && c < 0x10000)
				io.AddInputCharacter((unsigned short)c);
		}

		bool Application::Create(const ApplicationDesc & Desc)
		{
			Core::Context::SetRenderAPI(Desc.renderer);

			glfwInit();

			if (Desc.renderer != RenderAPI::OpenGL3)
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
			else {
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			}
			window = glfwCreateWindow(Desc.width, Desc.height, Desc.title.c_str(), NULL, NULL);
			if (window == NULL)
			{
				Log.Error("[Application] Failed to create GLFW window: " + Desc.title + "\n");
				return false;
			}
			glfwHideWindow(window);
			if (Desc.renderer == RenderAPI::OpenGL3)
			{
				glfwMakeContextCurrent(window);
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					Log.Error("[Application] Failed to initialize GLAD in window: " + Desc.title + "\n");
					return false;
				}

			}

			if (Desc.renderer == RenderAPI::DirectX11)
			{
				if (!API::DirectX::DX11Context::Initialize(window))
				{
					Log.Error("[Application] Failed to initialize DirectX11 in window: " + Desc.title + "\n");
					return false;
				}
			}
			ImGui_Impl_Init(window);

			if (NuclearEngine::Core::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::OpenGL3)
			{
				ImGui_ImplGL3_Init();
			}
			else if (NuclearEngine::Core::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::DirectX11)
			{
				ImGui_ImplDX11_Init(API::DirectX::DX11Context::GetDevice(), API::DirectX::DX11Context::GetContext());
			}

			ImGui_Impl_CreateDeviceObjects();
			//Install Callbacks
			glfwSetWindowSizeCallback(window, window_size_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetKeyCallback(window, key_callback);
			glfwSetCharCallback(window, char_callback);

			Log.Info("[Application] Created Application: " + Desc.title + " Width: " +  std::to_string(Desc.width) + " Height: " + std::to_string(Desc.height) + " \n");
			return true;
		}
		void Application::Shutdown()
		{
			if (Context::GetRenderAPI() == RenderAPI::DirectX11)
			{
				API::DirectX::DX11Context::Shutdown();
			}
			glfwDestroyWindow(window);
			glfwTerminate();
		}
		void Application::Display()
		{
			glfwShowWindow(window);
		}
		void Application::Hide()
		{
			glfwHideWindow(window);
		}
		void Application::SwapBuffers()
		{
			if (Context::GetRenderAPI() == RenderAPI::OpenGL3)
			{
				glfwSwapBuffers(window);
			}
			else if (Context::GetRenderAPI() == RenderAPI::DirectX11)
			{
				API::DirectX::DX11Context::SwapBuffers();
			}
		}
		void Application::ProcessEvents()
		{
			glfwPollEvents();
		}
		void Application::SetSize(int width, int height)
		{
			glfwSetWindowSize(window, width, height);
		}
		void Application::SetTitle(std::string title)
		{
			glfwSetWindowTitle(window, title.c_str());
		}
		bool Application::ShouldClose()
		{
			return glfwWindowShouldClose(window);
		}
		void Application::SetMouseInputMode(const MouseInputMode & mode)
		{
			switch (mode)
			{
			case MouseInputMode::Normal:
				return glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			case MouseInputMode::Virtual:
				return glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			case MouseInputMode::Hidden:
				return glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}	
		void Application::GetSize(int* width, int* height)
		{
			glfwGetWindowSize(window,width, height);
		}
		float Application::GetAspectRatiof()
		{
			int width= 800, height = 600;
			GetSize(&width, &height);
			return static_cast<float>(width) / static_cast<float>(height);
		}
		HWND Application::GetHandle()
		{
			return glfwGetWin32Window(window);
		}
	}
}
