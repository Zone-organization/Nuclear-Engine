#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual, Normal, Hidden };

		enum class RenderAPI : Uint8
		{
			OpenGL4_5,
			DirectX11
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			Uint32 Samples = 2;
			RenderAPI Renderer = RenderAPI::OpenGL4_5;
			bool DebugRenderAPI = false;
			bool V_Sync = true;
			std::wstring Title = L"NuclearEngine";
		};
	}
}
