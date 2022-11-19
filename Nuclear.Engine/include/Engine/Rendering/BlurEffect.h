#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <Engine\Graphics\RenderTarget.h>
#include <Engine\Graphics\Color.h>
#include <Engine\Rendering\Skybox.h>
namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI BlurEffect
		{
		public:

			void Initialize(Uint32 RTWidth, Uint32 RTHeight);

			void SetHorizentalPSO(ITextureView* texture);

			void SetVerticalPSO(ITextureView* texture);

			void ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight);

		//private:
			RefCntAutoPtr<IPipelineState> mHorzBlurPSO;
			RefCntAutoPtr<IShaderResourceBinding> mHorzBlurSRB;

			RefCntAutoPtr<IPipelineState> mVertBlurPSO;
			RefCntAutoPtr<IShaderResourceBinding> mVertBlurSRB;

			RefCntAutoPtr<IBuffer> mBlurCB;

			Graphics::RenderTarget BlurHorizentalRT;
			Graphics::RenderTarget BlurVerticalRT;
		};
	}
}