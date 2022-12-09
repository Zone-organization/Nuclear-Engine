#include "Graphics\GraphicsEngine.h"
#include <Graphics\Context.h>
#include <Managers\AssetManager.h>
#include "..\Graphics\ImGUI\imgui_impl_sdl.h"
#include "..\Graphics\ImGUI\imgui_impl.h"
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Graphics
	{
		inline GraphicsEngine& GraphicsEngine::GetInstance()
		{
			static GraphicsEngine engine;

			return engine;
		}
		bool GraphicsEngine::Initialize(const GraphicsEngineDesc & desc)
		{
			//Initialize Context
			if (!Graphics::Context::GetInstance().Initialize(desc))
			{
				NUCLEAR_INFO("[GraphicsEngine] Failed to initialize Context!");
				return false;
			}
			
			GetShaderManager().Initialize();
			Assets::DefaultMeshes::Initialize();

			if (desc.InitImGui)
			{
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.Fonts->AddFontFromFileTTF(Core::Path("@NuclearAssets@/Fonts/Roboto-Medium.ttf").GetRealPath().c_str(), 15);
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				io.ConfigDockingWithShift = true;
				ImGui_ImplSDL2_InitForDiligent(desc.pWindowHandle);
				ImGui_Impl_Init();
				ImGui_Impl_CreateDeviceObjects();
				//io.Fonts->GetTexDataAsAlpha8()

				NUCLEAR_INFO("[GraphicsEngine] ImGUI Initalized.");
			}

			NUCLEAR_INFO("[GraphicsEngine] GraphicsEngine has has been initalized successfully!.");
			return true;
		}
		void GraphicsEngine::Shutdown()
		{
			NUCLEAR_INFO("[GraphicsEngine] Shutting down...");
			Graphics::Context::GetInstance().Shutdown();
		}
		Managers::ShaderManager& GraphicsEngine::GetShaderManager()
		{
			return mDefaultShaderManager;
		}

		bool GraphicsEngine::isGammaCorrect()
		{
			if (Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat == TEX_FORMAT_RGBA8_UNORM_SRGB)
				return true;
			else
				return false;
		}

		GraphicsEngine::GraphicsEngine()
		{
		}
		
	}
}