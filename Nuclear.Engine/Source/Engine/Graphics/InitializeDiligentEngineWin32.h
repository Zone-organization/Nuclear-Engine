#pragma once
#include <Core\Logger.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/SwapChain.h>
#include "Diligent/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Diligent/Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Diligent/Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Diligent/Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
#include <SDL\include\SDL.h>
#include <SDL\include\SDL_syswm.h>

namespace Nuclear 
{
	void DiligentMassageCallback(DEBUG_MESSAGE_SEVERITY Severity, const Char* Message, const char* Function, const char* File, int Line)
	{
		switch (Severity)
		{
		case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_INFO:
			NUCLEAR_INFO("[GraphicsEngine] '{0}'" , Message);
			return;
		case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_WARNING:
			NUCLEAR_WARN("[GraphicsEngine] '{0}'", Message);

			return;
		case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_ERROR:
			NUCLEAR_ERROR("[GraphicsEngine] '{0}'", Message);

			return;
		case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
			NUCLEAR_FATAL("[GraphicsEngine] '{0}'", Message);
			return;
		default:
			return;
		};

	}

	bool InitializeDiligentEngineWin32(SDL_Window* window,
		const RENDER_DEVICE_TYPE& type,
		IRenderDevice** device,
		IDeviceContext** context,
		ISwapChain** swapchain,
		IEngineFactory** factory,
		const SwapChainDesc& SCDesc
	)
	{
		Uint32 NumDeferredCtx = 0;

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);

		FullScreenModeDesc FSDesc;
		NativeWindow DLWindow;
		DLWindow.hWnd = wmInfo.info.win.window;

		switch (type)
		{
		case RENDER_DEVICE_TYPE_D3D11:
		{
			EngineD3D11CreateInfo DeviceCreateInfo;
#if ENGINE_DLL
			GetEngineFactoryD3D11Type GetEngineFactoryD3D11 = nullptr;
			// Load the dll and import GetEngineFactoryD3D11() function
			LoadGraphicsEngineD3D11(GetEngineFactoryD3D11);
#endif
			auto *pFactoryD3D11 = GetEngineFactoryD3D11();
			*factory = pFactoryD3D11;
			pFactoryD3D11->CreateDeviceAndContextsD3D11(DeviceCreateInfo, device,
				context);
			pFactoryD3D11->CreateSwapChainD3D11(*device, *context,
				SCDesc, FSDesc, DLWindow, swapchain);
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
			EngineD3D12CreateInfo EngD3D12CreateInfo;

			auto *pFactoryD3D12 = GetEngineFactoryD3D12();
			*factory = pFactoryD3D12;

			pFactoryD3D12->CreateDeviceAndContextsD3D12(EngD3D12CreateInfo, device,
				context);
			pFactoryD3D12->CreateSwapChainD3D12(*device, *context,
				SCDesc, FSDesc, DLWindow, swapchain);
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
			auto *pFactoryOpenGL = GetEngineFactoryOpenGL();
			*factory = pFactoryOpenGL;

			EngineGLCreateInfo CreationCreateInfo;
			CreationCreateInfo.Window = DLWindow;
			pFactoryOpenGL->CreateDeviceAndSwapChainGL(
				CreationCreateInfo, device, context, SCDesc, swapchain);
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

			auto *pFactoryVk = GetEngineFactoryVk();
			*factory = pFactoryVk;

			pFactoryVk->CreateDeviceAndContextsVk(EngVkCreateInfo, device,
				context);
			pFactoryVk->CreateSwapChainVk(*device, *context,
				SCDesc, DLWindow, swapchain);
			pFactoryVk->SetMessageCallback(DiligentMassageCallback);

		}
		break;

		default:
			NUCLEAR_FATAL("[InitializeDiligentEngineWin32] Unknown device type!");
			return false;
		}

		return true;
	}
}