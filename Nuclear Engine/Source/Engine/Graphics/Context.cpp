#include <Engine\Graphics\Context.h>
#include <Core/Application.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		static std::unique_ptr<LLGL::RenderSystem> gRenderer;
		static LLGL::RenderContext*  gContext = nullptr;
		static LLGL::CommandBuffer*  gCommands = nullptr; 		// Main command buffer
		static LLGL::CommandBufferExt*  gCommandsExt = nullptr;		// Main command buffer
		static LLGL::CommandQueue* gCommandQueue = nullptr; 		// Command queue
		bool OpenGL = false;
		bool Vulkan = false;
		bool Direct3D = false;
		bool Metal = false;


		bool Context::Initialize(const Core::ApplicationDesc & Desc, LLGL::Window* window)
		{
			/* Select renderer module */
			std::string rendererModule;
			
			/* Find available modules */
			auto modules = LLGL::RenderSystem::FindModules();

			if (modules.empty())
			{
				/* No modules available -> throw error */
				Log.FatalError("[Context] No renderer modules available on target platform.\n");
				return false;
			}
			else if (modules.size() == 1)
			{
				/* Use the only available module */
				rendererModule = modules.front();
			}
			else
			{
				// Let user select a renderer */
				if (Desc.AutoSelectRenderer == false)
				{
					while (rendererModule.empty())
					{
						/* Print list of available modules */
						std::cout << "-- [Context] -- Choose A Renderer : " << "\n";

						int i = 0;
						for (const auto& mod : modules)
							std::cout << " " << (++i) << ".) " << mod << "\n";

						/* Wait for user input */
						std::size_t selection = 0;
						std::cin >> selection;
						--selection;

						if (selection < modules.size())
							rendererModule = modules[selection];
						else
							std::cerr << "invalid input, setting " << rendererModule << " as default.\n";
					}
				}
				else
				{
					switch (Desc.Renderer)
					{
					case Core::RenderAPI::DirectX11:
						rendererModule = "Direct3D11";
					case Core::RenderAPI::DirectX12:
						rendererModule = "Direct3D12";
					case Core::RenderAPI::Vulkan:
						rendererModule = "Vulkan";
					case Core::RenderAPI::OpenGL4:
						rendererModule = "OpenGL";
					}
				}
			}

			/* Choose final renderer module */
			gRenderer = LLGL::RenderSystem::Load(rendererModule);

			// Create render context
			LLGL::RenderContextDescriptor contextDesc;
			{
				contextDesc.videoMode.resolution = { Desc.WindowWidth, Desc.WindowHeight };
				contextDesc.vsync.enabled = Desc.VSync;
				contextDesc.videoMode.fullscreen = Desc.FullScreen;
				contextDesc.profileOpenGL.contextProfile = LLGL::OpenGLContextProfile::CoreProfile;
#ifdef ENABLE_MULTISAMPLING
				contextDesc.multiSampling = LLGL::MultiSamplingDescriptor{ 8 }; // check if LLGL adapts sample count that is too high
#endif
			}
			LLGL::RenderContext* context = gRenderer->CreateRenderContext(contextDesc);

			// Print renderer information
			const auto& info = gRenderer->GetRendererInfo();

			Log.Info("[Context] LLGL Renderer Initialized Successfully.\n");
			Log.Info("[Context] Renderer:         " + info.rendererName + "\n");
			Log.Info("[Context] Device:           " + info.deviceName + "\n");
			Log.Info("[Context] Vendor:           " + info.vendorName + "\n");
			Log.Info("[Context] Shading Language: " + info.shadingLanguageName + "\n");

			auto& _window = static_cast<LLGL::Window&>(gContext->GetSurface());

			return true;

		}

		void Context::PostInitialize()
		{
			if (gRenderer->GetRendererID() == LLGL::RendererID::OpenGL)
				OpenGL = true;
			else if (gRenderer->GetRendererID() == LLGL::RendererID::Vulkan)
				Vulkan = true;
			else if (gRenderer->GetRendererID() == LLGL::RendererID::Direct3D9 ||
				gRenderer->GetRendererID() == LLGL::RendererID::Direct3D10 ||
				gRenderer->GetRendererID() == LLGL::RendererID::Direct3D11 ||
				gRenderer->GetRendererID() == LLGL::RendererID::Direct3D12)
				Direct3D = true;
			else if (gRenderer->GetRendererID() == LLGL::RendererID::Metal)
				Metal = true;

			return;
		}

		void Context::PresentFrame()
		{
			return gContext->Present();
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
			return gRenderer.get();
		}

		LLGL::RenderContext * Context::GetRenderContext()
		{
			return gContext;
		}

		LLGL::CommandBuffer * Context::GetCommands()
		{
			return gCommands;
		}

		LLGL::CommandBufferExt * Context::GetCommandsExt()
		{
			return gCommandsExt;
		}

		LLGL::CommandQueue * Context::GetCommandQueue()
		{
			return gCommandQueue;
		}

	}
}