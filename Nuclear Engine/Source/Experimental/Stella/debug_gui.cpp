#include "stella/graphics/debug_gui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

DebugGui::DebugGui() {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
}

DebugGui::~DebugGui() {
  ImGui_ImplSdlGL3_Shutdown();
  ImGui::DestroyContext();
}

void DebugGui::Init(SDL_Window *window) {
  this->Window = window;
  ImGui_ImplSdlGL3_Init(this->Window);
  ImGui::StyleColorsDark();
}

void DebugGui::Update() {
  ImGui_ImplSdlGL3_NewFrame(this->Window);

  // Create a window called "My First Tool", with a menu bar.
  ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
  if (ImGui::BeginMenuBar())
  {
      if (ImGui::BeginMenu("File"))
      {
          if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
          if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
          if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
          ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
  }

  // Edit a color (stored as ~4 floats)
  float my_color[4]{0.0,0.0,0.0,0.0};
  ImGui::ColorEdit4("Color", my_color);

  // Plot some values
  const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
  ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));
   
  // Display contents in a scrolling region
  ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
  ImGui::BeginChild("Scrolling");
  for (int n = 0; n < 5; n++)
      ImGui::Text("%04d: Some text", n);
  ImGui::EndChild();
  ImGui::End();

  ImGui::Render();
  ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGui::GetInput(SDL_Event& e) {
  ImGui_ImplSdlGL3_ProcessEvent(&e);
}

