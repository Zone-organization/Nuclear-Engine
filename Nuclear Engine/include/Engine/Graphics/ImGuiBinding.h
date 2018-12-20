#pragma once
#include <Engine/Graphics/ImGui.h>
#include <LLGL/Window.h>

namespace NuclearEngine
{
	namespace Graphics
	{		
		class NEAPI ImGuiBinding
		{
		public:
			void InitializeWindow(LLGL::Window * window);
			//void ProcessEvent(const sf::Event& event);
			void Update(LLGL::Window* window, float dt);

			static bool Initialize();
			static void Shutdown();
			static void NewFrame();
			static void RenderDrawData(ImDrawData* draw_data);

			// Use if you want to reset your rendering device without losing ImGui state.
			static void CreateFontsTexture();
			static void DestroyFontsTexture();

		};
	}
}
