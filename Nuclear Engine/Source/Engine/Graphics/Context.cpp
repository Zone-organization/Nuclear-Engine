#include <Engine\Graphics\Context.h>
#include <Core/Application.h>
#include "InitializeDiligentEngineWin32.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		static Diligent::IRenderDevice* gDevice;
		static Diligent::IDeviceContext* gContext;
		static Diligent::ISwapChain* gSwapChain;
		bool OpenGL = false;
		bool Vulkan = false;
		bool Direct3D = false;
		bool Metal = false;


		bool Context::Initialize(const Core::ApplicationDesc & Desc)
		{
			InitializeDiligentEngineWin32(Core::Application::GetWindow(),static_cast<Diligent::DeviceType>(Desc.Renderer), gDevice, gContext, gSwapChain);
			Log.Info("[Context] Diligent Graphics API Initialized.\n");
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

		Diligent::IRenderDevice * Context::GetDevice()
		{
			return gDevice;
		}

		Diligent::IDeviceContext * Context::GetContext()
		{
			return gContext;
		}

		Diligent::ISwapChain * Context::GetSwapChain()
		{
			return gSwapChain;
		}
	}
}