// ImGui GLFW binding with OpenGL3 + shaders
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui
#include <Engine\GUI\imgui.h>

bool ImGui_ImplGL3_Init();
void ImGui_ImplGL3_Shutdown();

// Use if you want to reset your rendering device without losing ImGui state.
void ImGui_ImplGL3_InvalidateDeviceObjects();
bool ImGui_ImplGL3_CreateDeviceObjects();
