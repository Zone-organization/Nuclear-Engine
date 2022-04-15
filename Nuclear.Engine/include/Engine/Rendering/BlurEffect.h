#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <Engine\Graphics\RenderTarget.h>
#include <Engine\Graphics\Color.h>
#include <Engine\Graphics\CompoundPipeline.h>
#include <Engine\Rendering\ShaderEffect.h>
#include <Engine\Rendering\Skybox.h>
namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI BlurEffect
		{
		public:

			void Initialize(Uint32 RTWidth, Uint32 RTHeight);
		//private:
			RefCntAutoPtr<IPipelineState> mBlurPSO;
			RefCntAutoPtr<IShaderResourceBinding> mBlurSRB;
			RefCntAutoPtr<IBuffer> mBlurCB;

			Graphics::RenderTarget BlurHorizentalRT;
			Graphics::RenderTarget BlurVerticalRT;
		};
	}
}