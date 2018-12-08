#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual, Normal, Hidden };

		enum class RenderAPI : Uint8
		{
			DirectX11,
			DirectX12,
			OpenGL4,
			Vulkan
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			Uint32 Samples = 2;
			bool FullScreen = false;
			RenderAPI Renderer = RenderAPI::DirectX11;
			bool DebugRenderAPI = false;
			bool VSync = true;
			bool AutoSelectRenderer = true;
			std::wstring Title = L"NuclearEngine";
		};
	}
}
