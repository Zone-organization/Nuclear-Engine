#pragma once
#include <Core\NE_Common.h>
#include <Engine\Managers\ShaderManager.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			//The shader manager used internally by the engine if this is a nullptr the engine auto create a one.
		//	Managers::ShaderManager* DefaultShaderManager = nullptr;

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