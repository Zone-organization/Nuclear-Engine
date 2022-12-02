#pragma once
#include <Core\NE_Common.h>
#include <Engine\Managers\ShaderManager.h>

struct GLFWwindow;

namespace Nuclear
{
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			RENDER_DEVICE_TYPE mRenderAPI = RENDER_DEVICE_TYPE::RENDER_DEVICE_TYPE_D3D11;

			bool InitImGui = true;
			GLFWwindow* pWindowHandle;

			SwapChainDesc SCDesc;

			//Note: Overrides SCDesc ColorBufferFormat
			bool GammaCorrect = false;
		};

		class NEAPI GraphicsEngine
		{
		public:
			GraphicsEngine(GraphicsEngine const&) = delete;
			void operator=(GraphicsEngine const&) = delete;

			static GraphicsEngine& GetInstance();

			bool Initialize(const GraphicsEngineDesc& desc);
			void Shutdown();
			Managers::ShaderManager& GetShaderManager();

			bool isGammaCorrect();

		protected:
			Managers::ShaderManager mDefaultShaderManager;

		private:
			GraphicsEngine();
		};
	}
}