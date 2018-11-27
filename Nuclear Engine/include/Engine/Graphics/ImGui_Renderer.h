#pragma once
#include <Engine/Graphics/ImGui.h>
#include "Engine/Graphics/API/RenderAPI.h"

namespace NuclearEngine
{
	namespace Graphics
	{		
		class NEAPI ImGui_Renderer
		{
		public:
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
