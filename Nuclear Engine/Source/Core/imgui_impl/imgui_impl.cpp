#include "imgui_impl.h"

#include <Graphics\API\Context.h>
#include <GLFW\include\GLFW\glfw3.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW\include\GLFW\glfw3native.h>
#endif

static GLFWwindow*  g_Window = NULL;
static double       g_Time = 0.0f;
static bool         g_MouseJustPressed[3] = { false, false, false };
static float        g_MouseWheel = 0.0f;

static const char* getclipboardtext(void* user_data)
{
	return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void setclipboardtext(void* user_data, const char* text)
{
	glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void Imgui_Scroll_Callback(double yoffset)
{
	g_MouseWheel += (float)yoffset; // Use fractional mouse wheel.
}

void Imgui_mouse_button_callback(int button, int action)
{
	if (action == GLFW_PRESS && button >= 0 && button < 3)
	{
		g_MouseJustPressed[button] = true;
	}
}

bool ImGui_Impl_Init(GLFWwindow * window)
{
	g_Window = window;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	//io.RenderDrawListsFn = ImGui_ImplGlfwGL3_RenderDrawLists;       // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    io.SetClipboardTextFn = setclipboardtext;
	io.GetClipboardTextFn = getclipboardtext;
	io.ClipboardUserData = g_Window;
#ifdef _WIN32
	io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif

	return false;
}

void ImGui_Impl_Shutdown()
{
	if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::OpenGL3)
	{
		return ImGui_ImplGL3_Shutdown();
	}
	else if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::DirectX11)
	{
		return ImGui_ImplDX11_Shutdown();
	}
}


void ImGui::NE_NewFrame()
{
	return ImGui_Impl_NewFrame();
}

void ImGui_Impl_NewFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(g_Window, &w, &h);
	glfwGetFramebufferSize(g_Window, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
	if (glfwGetWindowAttrib(g_Window, GLFW_FOCUSED))
	{
		if (io.WantMoveMouse)
		{
			glfwSetCursorPos(g_Window, (double)io.MousePos.x, (double)io.MousePos.y);   // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
			io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Get mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
		}
	}
	else
	{
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	for (int i = 0; i < 3; i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
		g_MouseJustPressed[i] = false;
	}

	io.MouseWheel = g_MouseWheel;
	g_MouseWheel = 0.0f;

	// Hide OS mouse cursor if ImGui is drawing it
	glfwSetInputMode(g_Window, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

	// Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
	ImGui::NewFrame();
}

void ImGui_Impl_InvalidateDeviceObjects()
{
	if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::OpenGL3)
	{
		return ImGui_ImplGL3_InvalidateDeviceObjects();
	}
	else if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::DirectX11)
	{
		return ImGui_ImplDX11_InvalidateDeviceObjects();
	}
}

bool ImGui_Impl_CreateDeviceObjects()
{
	if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::OpenGL3)
	{
		return ImGui_ImplGL3_CreateDeviceObjects();
	}
	else if (NuclearEngine::Graphics::API::Context::GetRenderAPI() == NuclearEngine::Core::RenderAPI::DirectX11)
	{
		return ImGui_ImplDX11_CreateDeviceObjects();
	}
}
