#include <Graphics\Context.h>
//#include <Engine.h>
//#include <Core/Window.h>
#include "InitializeDiligentEngineWin32.h"
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Graphics
	{
		inline Context& Context::GetInstance()
		{
			static Context context;

			return context;
		}

		bool Context::Initialize(const Graphics::GraphicsEngineDesc& GraphicsDesc)
		{
			SwapChainDesc SCDesc(GraphicsDesc.SCDesc);

			if (GraphicsDesc.GammaCorrect)
			{
				SCDesc.ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
			}
			else
			{
				SCDesc.ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM;
			}
			bool Result = InitializeDiligentEngineWin32(GraphicsDesc.pWindowHandle, GraphicsDesc.mRenderAPI, &gDevice, &gContext, &gSwapChain,&gEngineFactory, SCDesc);

			if (!Result)
			{
				NUCLEAR_ERROR("[Context] Failed to initialize DiligentCore!");
				return false;
			}

			NUCLEAR_INFO("[Context] Diligent Graphics API Initialized.");
			return true;
		}

		void Context::Shutdown()
		{
			NUCLEAR_INFO("[Context] Shutting down...");
			gDevice->Release();
			gContext->Release();
			gSwapChain->Release();
		}

		void Context::PresentFrame()
		{
			gSwapChain->Present();
		}

		bool Context::IsOpenGL()
		{
			return OpenGL;
		}

		bool Context::IsVulkan()
		{
			return Vulkan;
		}

		bool Context::IsDirect3D()
		{
			return Direct3D;
		}

		void Context::ResizeSwapChain(Uint32 Width, Uint32 Height)
		{
			if (gSwapChain)
				gSwapChain->Resize(Width, Height);
		}

		IRenderDevice * Context::GetDevice()
		{
			return gDevice;
		}

		IDeviceContext * Context::GetContext()
		{
			return gContext;
		}

		ISwapChain * Context::GetSwapChain()
		{
			return gSwapChain;
		}
		IEngineFactory* Context::GetEngineFactory()
		{
			return gEngineFactory;
		}
		Context::Context()
		{
			gDevice = nullptr;
			gContext = nullptr;
			gSwapChain = nullptr;
			gEngineFactory = nullptr;
		}
	}
}