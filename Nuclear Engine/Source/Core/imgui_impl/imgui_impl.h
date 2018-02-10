#pragma once
#include "imgui_impl_dx11.h"
#include "imgui_impl_gl3.h"
struct GLFWwindow;

bool ImGui_Impl_Init(GLFWwindow* window);

void ImGui_Impl_Shutdown();
void ImGui_Impl_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
void ImGui_Impl_InvalidateDeviceObjects();
bool ImGui_Impl_CreateDeviceObjects();

void Imgui_Scroll_Callback(double yoffset);

void Imgui_mouse_button_callback(int button, int action);