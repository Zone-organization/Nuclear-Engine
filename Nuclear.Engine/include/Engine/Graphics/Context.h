#pragma once
#include <Core\NE_Common.h>
#include <Core\Window.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Diligent/Graphics/GraphicsEngine/interface/EngineFactory.h>
#include "Engine\Graphics\GraphicsEngine.h"

namespace Nuclear
{
	namespace Graphics 
	{
		class NEAPI Context
		{
		public:
			Context(Context const&) = delete;
			void operator=(Context const&) = delete;

			static Context& GetInstance();

			bool Initialize(const Graphics::GraphicsEngineDesc& GraphicsDesc);

			void Shutdown();

			void PresentFrame();

			bool IsOpenGL();			// Returns ture if OpenGL is used as rendering API.

			bool IsVulkan();			// Returns ture if Vulkan is used as rendering API.
			
			bool IsDirect3D();		// Returns ture if Direct3D is used as rendering API.
			
			void ResizeSwapChain(Uint32 Width, Uint32 Height);

			IRenderDevice* GetDevice();
			IDeviceContext* GetContext();
			ISwapChain* GetSwapChain();
			IEngineFactory* GetEngineFactory();

		private:
			Context();
			IRenderDevice* gDevice;
			IDeviceContext* gContext;
			ISwapChain* gSwapChain;
			IEngineFactory* gEngineFactory;

			bool OpenGL = false;
			bool Vulkan = false;
			bool Direct3D = false;
			bool Metal = false;
		};
	}
}