#pragma once
#include <Engine/Graphics/ImGui.h>
#include "Engine/Graphics/API/RenderAPI.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		class ImGui_Renderer
		{
		public:
			static bool Initialize();
			static void Shutdown();
			static void NewFrame();
			static void RenderDrawData(ImDrawData* draw_data);

			// Use if you want to reset your rendering device without losing ImGui state.
			static void InvalidateDeviceObjects();
			static bool CreateDeviceObjects();


			//static int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
		};
	}
}
