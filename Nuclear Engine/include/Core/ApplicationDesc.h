#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Core {

		enum class RenderAPI : Int32
		{
			Undefined = 0,  ///< Undefined device
			DirectX11,      ///< D3D11 device
			DirectX12,      ///< D3D12 device
			OpenGL,     ///< OpenGL device 
			OpenGLES,   ///< OpenGLES device
			Vulkan      ///< Vulkan device
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1280;
			Uint32 WindowHeight = 720;
			Uint32 Samples = 2;
			bool FullScreen = false;
			RenderAPI Renderer = RenderAPI::DirectX11;
			bool DebugRenderAPI = false;
			bool VSync = true;
			bool AutoSelectRenderer = true;
			std::string Title = "NuclearEngine";
		};
	}
}
