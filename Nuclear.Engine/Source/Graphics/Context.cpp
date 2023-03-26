#include <Graphics\Context.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/SwapChain.h>
#include "Diligent/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Diligent/Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Diligent/Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Diligent/Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
#include <SDL\include\SDL.h>
#include <SDL\include\SDL_syswm.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Graphics
	{
		using namespace Diligent;
		void DiligentMassageCallback(DEBUG_MESSAGE_SEVERITY Severity, const Char* Message, const char* Function, const char* File, int Line)
		{
			switch (Severity)
			{
			case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_INFO:
				NUCLEAR_INFO("[Diligent] {0}", Message);
				return;
			case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_WARNING:
				NUCLEAR_WARN("[Diligent] {0}", Message);

				return;
			case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_ERROR:
				NUCLEAR_ERROR("[Diligent] {0}", Message);

				return;
			case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
				NUCLEAR_FATAL("[Diligent] {0}", Message);
				return;
			default:
				return;
			};

		}

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
			bool Result = InitializeDiligentEngine(GraphicsDesc.pWindowHandle, GraphicsDesc.mRenderAPI, SCDesc);

			if (!Result)
			{
				NUCLEAR_ERROR("[Context] Failed to initialize DiligentCore!");
				return false;
			}

			auto deviceinfo = gDevice->GetAdapterInfo();

			NUCLEAR_INFO("[Context] Diligent Graphics API Initialized On: {0}", deviceinfo.Description);
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
		bool Context::InitializeDiligentEngine(SDL_Window* window, const RENDER_DEVICE_TYPE& type, const SwapChainDesc& SCDesc)
		{
			Uint32 NumDeferredCtx = 0;

			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(window, &wmInfo);

			FullScreenModeDesc FSDesc;
			NativeWindow DLWindow;
			DLWindow.hWnd = wmInfo.info.win.window;

			auto FindAdapter = [this](auto* pFactory, Version GraphicsAPIVersion, GraphicsAdapterInfo& AdapterAttribs) {
				Uint32 NumAdapters = 0;
				pFactory->EnumerateAdapters(GraphicsAPIVersion, NumAdapters, nullptr);
				std::vector<GraphicsAdapterInfo> Adapters(NumAdapters);
				if (NumAdapters > 0)
					pFactory->EnumerateAdapters(GraphicsAPIVersion, NumAdapters, Adapters.data());
				else
					LOG_ERROR_AND_THROW("Failed to find compatible hardware adapters");

				auto AdapterId = mAdapterId;
				if (AdapterId != DEFAULT_ADAPTER_ID)
				{
					if (AdapterId < Adapters.size())
					{
						mAdapterType = Adapters[AdapterId].Type;
					}
					else
					{
						LOG_ERROR_MESSAGE("Adapter ID (", AdapterId, ") is invalid. Only ", Adapters.size(), " compatible ", (Adapters.size() == 1 ? "adapter" : "adapters"), " present in the system");
						AdapterId = DEFAULT_ADAPTER_ID;
					}
				}

				if (AdapterId == DEFAULT_ADAPTER_ID && mAdapterType != ADAPTER_TYPE_UNKNOWN)
				{
					for (Uint32 i = 0; i < Adapters.size(); ++i)
					{
						if (Adapters[i].Type == mAdapterType)
						{
							AdapterId = i;
							break;
						}
					}
					if (AdapterId == DEFAULT_ADAPTER_ID)
						LOG_WARNING_MESSAGE("Unable to find the requested adapter type. Using default adapter.");
				}

				if (AdapterId == DEFAULT_ADAPTER_ID)
				{
					mAdapterType = ADAPTER_TYPE_UNKNOWN;
					for (Uint32 i = 0; i < Adapters.size(); ++i)
					{
						const auto& AdapterInfo = Adapters[i];
						const auto  AdapterType = AdapterInfo.Type;
						static_assert((ADAPTER_TYPE_DISCRETE > ADAPTER_TYPE_INTEGRATED &&
							ADAPTER_TYPE_INTEGRATED > ADAPTER_TYPE_SOFTWARE &&
							ADAPTER_TYPE_SOFTWARE > ADAPTER_TYPE_UNKNOWN),
							"Unexpected ADAPTER_TYPE enum ordering");
						if (AdapterType > mAdapterType)
						{
							// Prefer Discrete over Integrated over Software
							mAdapterType = AdapterType;
							AdapterId = i;
						}
						else if (AdapterType == mAdapterType)
						{
							// Select adapter with more memory
							const auto& NewAdapterMem = AdapterInfo.Memory;
							const auto  NewTotalMemory = NewAdapterMem.LocalMemory + NewAdapterMem.HostVisibleMemory + NewAdapterMem.UnifiedMemory;
							const auto& CurrAdapterMem = Adapters[AdapterId].Memory;
							const auto  CurrTotalMemory = CurrAdapterMem.LocalMemory + CurrAdapterMem.HostVisibleMemory + CurrAdapterMem.UnifiedMemory;
							if (NewTotalMemory > CurrTotalMemory)
							{
								AdapterId = i;
							}
						}
					}
				}

				if (AdapterId != DEFAULT_ADAPTER_ID)
				{
					AdapterAttribs = Adapters[AdapterId];
					LOG_INFO_MESSAGE("Using adapter ", AdapterId, ": '", AdapterAttribs.Description, "'");
				}

				return AdapterId;
			};

			switch (type)
			{
			case RENDER_DEVICE_TYPE_D3D11:
			{
				EngineD3D11CreateInfo EngineCI;
#if ENGINE_DLL
				GetEngineFactoryD3D11Type GetEngineFactoryD3D11 = nullptr;
				// Load the dll and import GetEngineFactoryD3D11() function
				LoadGraphicsEngineD3D11(GetEngineFactoryD3D11);
#endif
				auto* pFactoryD3D11 = GetEngineFactoryD3D11();

				EngineCI.GraphicsAPIVersion = { 11, 0 };
				gEngineFactory = pFactoryD3D11;

				EngineCI.AdapterId = FindAdapter(pFactoryD3D11, EngineCI.GraphicsAPIVersion, mAdapterAttribs);

				pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &gDevice,
					&gContext);
				pFactoryD3D11->CreateSwapChainD3D11(gDevice, gContext,
					SCDesc, FSDesc, DLWindow, &gSwapChain);
				pFactoryD3D11->SetMessageCallback(DiligentMassageCallback);

			}
			break;

			case RENDER_DEVICE_TYPE_D3D12:
			{
#if ENGINE_DLL
				GetEngineFactoryD3D12Type GetEngineFactoryD3D12 = nullptr;
				// Load the dll and import GetEngineFactoryD3D12() function
				LoadGraphicsEngineD3D12(GetEngineFactoryD3D12);
#endif
				EngineD3D12CreateInfo EngineCI;
				EngineCI.GraphicsAPIVersion = { 11, 0 };

				auto* pFactoryD3D12 = GetEngineFactoryD3D12();
				gEngineFactory = pFactoryD3D12;
				EngineCI.AdapterId = FindAdapter(pFactoryD3D12, EngineCI.GraphicsAPIVersion, mAdapterAttribs);

				pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &gDevice,
					&gContext);
				pFactoryD3D12->CreateSwapChainD3D12(gDevice, gContext,
					SCDesc, FSDesc, DLWindow, &gSwapChain);
				pFactoryD3D12->SetMessageCallback(DiligentMassageCallback);
			}
			break;

			case RENDER_DEVICE_TYPE_GL:
			{

#if ENGINE_DLL
				// Declare function pointer
				GetEngineFactoryOpenGLType GetEngineFactoryOpenGL = nullptr;
				// Load the dll and import GetEngineFactoryOpenGL() function
				LoadGraphicsEngineOpenGL(GetEngineFactoryOpenGL);
#endif
				auto* pFactoryOpenGL = GetEngineFactoryOpenGL();
				gEngineFactory = pFactoryOpenGL;

				EngineGLCreateInfo CreationCreateInfo;
				CreationCreateInfo.Window = DLWindow;
				pFactoryOpenGL->CreateDeviceAndSwapChainGL(
					CreationCreateInfo, &gDevice, &gContext, SCDesc, &gSwapChain);
				pFactoryOpenGL->SetMessageCallback(DiligentMassageCallback);

			}
			break;

			case RENDER_DEVICE_TYPE_VULKAN:
			{
#if ENGINE_DLL
				GetEngineFactoryVkType GetEngineFactoryVk = nullptr;
				// Load the dll and import GetEngineFactoryVk() function
				LoadGraphicsEngineVk(GetEngineFactoryVk);
#endif
				EngineVkCreateInfo EngVkCreateInfo;

				auto* pFactoryVk = GetEngineFactoryVk();
				gEngineFactory = pFactoryVk;
				EngVkCreateInfo.AdapterId = FindAdapter(pFactoryVk, EngVkCreateInfo.GraphicsAPIVersion, mAdapterAttribs);

				pFactoryVk->CreateDeviceAndContextsVk(EngVkCreateInfo, &gDevice,
					&gContext);
				pFactoryVk->CreateSwapChainVk(gDevice, gContext,
					SCDesc, DLWindow, &gSwapChain);
				pFactoryVk->SetMessageCallback(DiligentMassageCallback);

			}
			break;

			default:
				NUCLEAR_FATAL("[InitializeDiligentEngineWin32] Unknown &gDevice type!");
				return false;
			}

			return true;
		}
	}
}