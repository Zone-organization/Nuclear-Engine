#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/SwapChain.h>
#include "Diligent/Graphics/GraphicsEngineD3D11/interface/RenderDeviceFactoryD3D11.h"
#include "Diligent/Graphics/GraphicsEngineD3D12/interface/RenderDeviceFactoryD3D12.h"
#include "Diligent/Graphics/GraphicsEngineOpenGL/interface/RenderDeviceFactoryOpenGL.h"
#include "Diligent/Graphics/GraphicsEngineVulkan/interface/RenderDeviceFactoryVk.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/include/GLFW/glfw3.h>
#include <GLFW/include/GLFW/glfw3native.h>
namespace NuclearEngine 
{
	bool InitializeDiligentEngineWin32(GLFWwindow* window,
		const DeviceType& type,
		IRenderDevice* device,
		IDeviceContext* context,
		ISwapChain* swapchain
	)
	{

		SwapChainDesc SCDesc;
		SCDesc.SamplesCount = 1;
		Uint32 NumDeferredCtx = 0;

		FullScreenModeDesc FSDesc;


		HWND NativeWindowHandle = glfwGetWin32Window(window);

		switch (type)
		{
		case DeviceType::D3D11:
		{
			EngineD3D11Attribs DeviceAttribs;
#if ENGINE_DLL
			GetEngineFactoryD3D11Type GetEngineFactoryD3D11 = nullptr;
			// Load the dll and import GetEngineFactoryD3D11() function
			LoadGraphicsEngineD3D11(GetEngineFactoryD3D11);
#endif
			auto *pFactoryD3D11 = GetEngineFactoryD3D11();
			pFactoryD3D11->CreateDeviceAndContextsD3D11(DeviceAttribs, &device,
				&context, NumDeferredCtx);
			pFactoryD3D11->CreateSwapChainD3D11(device, context,
				SCDesc, FSDesc, NativeWindowHandle, &swapchain);
		}
		break;

		case DeviceType::D3D12:
		{
#if ENGINE_DLL
			GetEngineFactoryD3D12Type GetEngineFactoryD3D12 = nullptr;
			// Load the dll and import GetEngineFactoryD3D12() function
			LoadGraphicsEngineD3D12(GetEngineFactoryD3D12);
#endif
			EngineD3D12Attribs EngD3D12Attribs;
			auto *pFactoryD3D12 = GetEngineFactoryD3D12();
			pFactoryD3D12->CreateDeviceAndContextsD3D12(EngD3D12Attribs, &device,
				&context, NumDeferredCtx);
			pFactoryD3D12->CreateSwapChainD3D12(device, context,
				SCDesc, FSDesc, NativeWindowHandle, &swapchain);
		}
		break;

		case DeviceType::OpenGL:
		{

#if ENGINE_DLL
			// Declare function pointer
			GetEngineFactoryOpenGLType GetEngineFactoryOpenGL = nullptr;
			// Load the dll and import GetEngineFactoryOpenGL() function
			LoadGraphicsEngineOpenGL(GetEngineFactoryOpenGL);
#endif
			auto *pFactoryOpenGL = GetEngineFactoryOpenGL();
			EngineGLAttribs CreationAttribs;
			CreationAttribs.pNativeWndHandle = NativeWindowHandle;
			pFactoryOpenGL->CreateDeviceAndSwapChainGL(
				CreationAttribs, &device, &context, SCDesc, &swapchain);
		}
		break;

		case DeviceType::Vulkan:
		{
#if ENGINE_DLL
			GetEngineFactoryVkType GetEngineFactoryVk = nullptr;
			// Load the dll and import GetEngineFactoryVk() function
			LoadGraphicsEngineVk(GetEngineFactoryVk);
#endif
			EngineVkAttribs EngVkAttribs;
			auto *pFactoryVk = GetEngineFactoryVk();
			pFactoryVk->CreateDeviceAndContextsVk(EngVkAttribs, &device,
				&context, NumDeferredCtx);
			pFactoryVk->CreateSwapChainVk(device, context,
				SCDesc, NativeWindowHandle, &swapchain);
		}
		break;

		default:
			Log.FatalError("[InitializeDiligentEngineWin32] Unknown device type!\n");
			return false;
		}

		return true;
	}
}