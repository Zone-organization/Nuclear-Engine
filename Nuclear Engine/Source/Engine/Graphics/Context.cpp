#include <Engine\Graphics\Context.h>
#include <Core/Application.h>
#include "InitializeDiligentEngineWin32.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		static IRenderDevice* gDevice;
		static IDeviceContext* gContext;
		static ISwapChain* gSwapChain;
		bool OpenGL = false;
		bool Vulkan = false;
		bool Direct3D = false;
		bool Metal = false;


		bool Context::Initialize(const Core::ApplicationDesc & Desc)
		{
			bool Result = InitializeDiligentEngineWin32(Core::Application::GetWindow(),static_cast<DeviceType>(Desc.Renderer), &gDevice, &gContext, &gSwapChain);

			if(Result)
				Log.Info("[Context] Diligent Graphics API Initialized.\n");

			return true;
		}

		void Context::ShutDown()
		{
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
	}
}