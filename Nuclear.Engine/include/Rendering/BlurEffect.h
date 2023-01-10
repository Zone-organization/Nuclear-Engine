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

			void SetHorizentalPSO(Diligent::ITextureView* texture);

			void SetVerticalPSO(Diligent::ITextureView* texture);

			void ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight);

		//private:
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mHorzBlurPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mHorzBlurSRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mVertBlurPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mVertBlurSRB;

			Diligent::RefCntAutoPtr<Diligent::IBuffer> mBlurCB;

			Graphics::RenderTarget BlurHorizentalRT;
			Graphics::RenderTarget BlurVerticalRT;
		};
	}
}