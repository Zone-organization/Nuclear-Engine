#pragma once
#include <Base\NE_Common.h>
#include <LLGL/LLGL.h>
#include <LLGL/Utility.h>
#include <Core/ApplicationDesc.h>

namespace NuclearEngine
{
	namespace Graphics 
	{
		class NEAPI Context
		{
		public:
			static bool Initialize(const Core::ApplicationDesc & Desc);

			static void PostInitialize();

			static void PresentFrame();

			static bool IsOpenGL();			// Returns ture if OpenGL is used as rendering API.

			static bool IsVulkan();			// Returns ture if Vulkan is used as rendering API.
			
			static bool IsDirect3D();		// Returns ture if Direct3D is used as rendering API.
									
			static bool IsMetal();			// Returns ture if Metal is used as rendering API.

			static LLGL::RenderSystem* GetRenderer();
			static LLGL::RenderContext* GetRenderContext();
			static LLGL::CommandBuffer* GetCommands();
			static LLGL::CommandBufferExt* GetCommandsExt();
			static LLGL::CommandQueue* GetCommandQueue();

		};
	}
}