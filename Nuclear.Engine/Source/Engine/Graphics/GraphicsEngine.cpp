#include "Engine\Graphics\GraphicsEngine.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Managers\AssetManager.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Graphics
	{
		GraphicsEngine& GraphicsEngine::GetInstance()
		{
			static GraphicsEngine engine;

			return engine;
		}
		bool GraphicsEngine::Initialize(const GraphicsEngineDesc & desc)
		{
			return true;
		}
		void GraphicsEngine::Shutdown()
		{
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