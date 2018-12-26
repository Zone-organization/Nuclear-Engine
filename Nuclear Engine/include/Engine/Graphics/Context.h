#pragma once
#include <Base\NE_Common.h>
#include <Core/ApplicationDesc.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/SwapChain.h>

namespace NuclearEngine
{
	namespace Graphics 
	{
		class NEAPI Context
		{
		public:
			static bool Initialize(const Core::ApplicationDesc & Desc);

			static void PresentFrame();

			static bool IsOpenGL();			// Returns ture if OpenGL is used as rendering API.

			static bool IsVulkan();			// Returns ture if Vulkan is used as rendering API.
			
			static bool IsDirect3D();		// Returns ture if Direct3D is used as rendering API.
			
			static void ResizeSwapChain(Uint32 Width, Uint32 Height);

			static Diligent::IRenderDevice* GetDevice();
			static Diligent::IDeviceContext* GetContext();
			static Diligent::ISwapChain* GetSwapChain();
		};
	}
}