// dear imgui: Renderer for Diligent

#pragma once
#include <Engine/Graphics/ImGUI.h>

IMGUI_IMPL_API bool  ImGui_Impl_Init();
IMGUI_IMPL_API void  ImGui_Impl_Shutdown();
IMGUI_IMPL_API void  ImGui_Impl_NewFrame();
IMGUI_IMPL_API void  ImGui_Impl_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_IMPL_API void  ImGui_Impl_InvalidateDeviceObjects();
IMGUI_IMPL_API bool  ImGui_Impl_CreateDeviceObjects();
