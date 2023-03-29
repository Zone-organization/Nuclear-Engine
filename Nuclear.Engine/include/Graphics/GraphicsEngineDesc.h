#pragma once
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>

struct SDL_Window;

namespace Nuclear
{
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			Diligent::RENDER_DEVICE_TYPE mRenderAPI = Diligent::RENDER_DEVICE_TYPE::RENDER_DEVICE_TYPE_D3D11;

			bool InitImGui = true;
			SDL_Window* pWindowHandle;

			Diligent::SwapChainDesc SCDesc;

			//Note: Overrides SCDesc ColorBufferFormat
			bool GammaCorrect = false;
		};

	}
}