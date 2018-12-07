#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual, Normal, Hidden };

		enum class RenderAPI : Uint8
		{
			DirectX11,
			DirectX12,
			OpenGL4_5,
			Vulkan
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			Uint32 Samples = 2;
			bool ManualSelectRenderer = false;
			bool AutoSelectRenderer = true;
			bool ForceRenderer = false;
			RenderAPI Renderer = RenderAPI::OpenGL4_5;
			bool DebugRenderAPI = false;
			bool VSync = true;
			std::wstring Title = L"NuclearEngine";
		};
	}
}
