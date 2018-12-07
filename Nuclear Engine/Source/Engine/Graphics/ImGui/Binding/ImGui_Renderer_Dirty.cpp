#include "Engine\Graphics\ImGui_Renderer.h"
#include "Engine\Graphics\API\Context.h"
#include "ImGui_GL4_Renderer.h"
#include "ImGui_DX11_Renderer.h"

namespace NuclearEngine
{
	namespace Graphics
	{


		bool ImGui_Renderer::Initialize()
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				return ImGui_GL4_Renderer_Initialize();
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				return ImGui_DX11_Renderer_Initialize();
			}
			
		}

		void ImGui_Renderer::Shutdown()
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				ImGui_GL4_Renderer_Shutdown();
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				ImGui_DX11_Renderer_Shutdown();
			}
		}
		void ImGui_Renderer::NewFrame()
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				ImGui_GL4_Renderer_NewFrame();
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				ImGui_DX11_Renderer_NewFrame();
			}
		}
		void ImGui_Renderer::RenderDrawData(ImDrawData * draw_data)
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				ImGui_GL4_Renderer_RenderDrawData(draw_data);
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				ImGui_DX11_Renderer_RenderDrawData(draw_data);
			}
		}

		void ImGui_Renderer::CreateFontsTexture()
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				ImGui_GL4_Renderer_CreateFontsTexture();
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				ImGui_DX11_Renderer_CreateFontsTexture();
			}
		}
		void ImGui_Renderer::DestroyFontsTexture()
		{
			if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::OpenGL4_5)
			{
				ImGui_GL4_Renderer_DestroyFontsTexture();
			}
			else if (LLGL::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				ImGui_DX11_Renderer_DestroyFontsTexture();
			}
		}
	}
}

