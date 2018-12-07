#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		static std::unique_ptr<LLGL::RenderSystem> mRenderer;
		static LLGL::RenderContext*  mContext = nullptr;
		static LLGL::CommandBuffer*  mCommands = nullptr; 		// Main command buffer
		static LLGL::CommandBufferExt*  mCommandsExt = nullptr;		// Main command buffer
		static LLGL::CommandQueue* mCommandQueue = nullptr; 		// Command queue
		bool OpenGL = false;
		bool Vulkan = false;
		bool Direct3D = false;
		bool Metal = false;


		void Context::PostInitialize()
		{
			if (mRenderer->GetRendererID() == LLGL::RendererID::OpenGL)
				OpenGL = true;
			else if (mRenderer->GetRendererID() == LLGL::RendererID::Vulkan)
				Vulkan = true;
			else if (mRenderer->GetRendererID() == LLGL::RendererID::Direct3D9 ||
				mRenderer->GetRendererID() == LLGL::RendererID::Direct3D10 ||
				mRenderer->GetRendererID() == LLGL::RendererID::Direct3D11 ||
				mRenderer->GetRendererID() == LLGL::RendererID::Direct3D12)
				Direct3D = true;
			else if (mRenderer->GetRendererID() == LLGL::RendererID::Metal)
				Metal = true;

			return;
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

		bool Context::IsMetal()
		{
			return Metal;
		}

		LLGL::RenderSystem* Context::GetRenderer()
		{
			return mRenderer.get();
		}

		LLGL::RenderContext * Context::GetRenderContext()
		{
			return mContext;
		}

		LLGL::CommandBuffer * Context::GetCommands()
		{
			return mCommands;
		}

		LLGL::CommandBufferExt * Context::GetCommandsExt()
		{
			return mCommandsExt;
		}

		LLGL::CommandQueue * Context::GetCommandQueue()
		{
			return mCommandQueue;
		}

	}
}