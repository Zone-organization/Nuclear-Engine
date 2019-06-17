#pragma once
#include <Base\NE_Common.h>
#include <Engine\Managers\ShaderManager.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			//The shader manager used internally by the engine if this is a nullptr the engine auto create a one.
			Managers::ShaderManager* DefaultShaderManager;

			SwapChainDesc SCDesc;

			//Note: Overrides SCDesc ColorBufferFormat
			bool GammaCorrect = false;
		};

		class NEAPI GraphicsEngine
		{
		public:
			static bool Initialize(const GraphicsEngineDesc& desc);
			static void Shutdown();
			static Managers::ShaderManager* GetShaderManager();

			static bool isGammaCorrect();

		protected:
			static Managers::ShaderManager* pDefaultShaderManager;
		};
	}
}