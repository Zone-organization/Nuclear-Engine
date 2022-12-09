#pragma once
#include <NE_Common.h>
#include <string>
#include <Graphics\RenderTarget.h>
#include <Graphics\Color.h>
#include <Rendering\Skybox.h>
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