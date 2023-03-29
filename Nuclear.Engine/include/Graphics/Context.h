#pragma once
#include <NE_Common.h>
#include <Graphics\GraphicsEngineDesc.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>

namespace Diligent
{
	struct ISwapChain;
	struct IEngineFactory;
}

namespace Nuclear
{
	namespace Graphics 
	{
		class NEAPI Context
		{
		public:
			Context(Context const&) = delete;
			void operator=(Context const&) = delete;

			FORCE_INLINE static Context& GetInstance()
			{
				static Context instance;

				return instance;
			}

			bool Initialize(const Graphics::GraphicsEngineDesc& GraphicsDesc);

			void Shutdown();

			void PresentFrame();

			bool IsOpenGL();			// Returns ture if OpenGL is used as rendering API.

			bool IsVulkan();			// Returns ture if Vulkan is used as rendering API.
			
			bool IsDirect3D();		// Returns ture if Direct3D is used as rendering API.
			
			void ResizeSwapChain(Uint32 Width, Uint32 Height);

			FORCE_INLINE Diligent::IRenderDevice* GetDevice() const
			{
				return pDevice;
			}

			FORCE_INLINE Diligent::IDeviceContext* GetContext() const
			{
				return pContext;
			}

			FORCE_INLINE Diligent::ISwapChain* GetSwapChain() const
			{
				return pSwapChain;
			}

			FORCE_INLINE Diligent::IEngineFactory* GetEngineFactory() const
			{
				return pEngineFactory;
			}


		protected:
			Context();

			Diligent::IRenderDevice* pDevice;
			Diligent::IDeviceContext* pContext;
			Diligent::ISwapChain* pSwapChain;
			Diligent::IEngineFactory* pEngineFactory;

			Diligent::GraphicsAdapterInfo mAdapterAttribs;
			Uint32       mAdapterId = Diligent::DEFAULT_ADAPTER_ID;
			Diligent::ADAPTER_TYPE mAdapterType = Diligent::ADAPTER_TYPE_UNKNOWN;
			std::string  mAdapterDetailsString;

			bool OpenGL = false;
			bool Vulkan = false;
			bool Direct3D = false;
			bool Metal = false;

			bool InitializeDiligentEngine(SDL_Window* window, const Diligent::RENDER_DEVICE_TYPE& type, const Diligent::SwapChainDesc& SCDesc);
		};
	}
}