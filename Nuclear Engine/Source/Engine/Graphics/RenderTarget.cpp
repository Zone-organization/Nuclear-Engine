#include <Engine\Graphics\RenderTarget.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		RenderTarget::RenderTarget()
		{
		}
		RenderTarget::~RenderTarget()
		{
		}
		void RenderTarget::Create(const RenderTargetDesc& Desc)
		{
			RefCntAutoPtr<ITexture> pRTColor;

			//Create Color Texture
			TextureDesc TexDesc;
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = Desc.Width;
			TexDesc.Height = Desc.Height;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
			TexDesc.Format = Desc.ColorTexFormat;
			TexDesc.MipLevels = 1;
			TexDesc.ClearValue = Desc.ClearValue;

			Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &pRTColor);
			mColorRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);
			mShaderRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

			//Create Depth Texture
			RefCntAutoPtr<ITexture> pRTDepth;
			TexDesc.Format = Desc.DepthTexFormat;
			TexDesc.ClearValue.Format = TexDesc.Format;
			TexDesc.ClearValue.DepthStencil.Depth = 1;
			TexDesc.ClearValue.DepthStencil.Stencil = 0;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

			Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &pRTDepth);
			mDepthDSV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);

		}

		void RenderTarget::SetActive(const float* RGBA)
		{		
			Graphics::Context::GetContext()->SetRenderTargets(1, mColorRTV.RawDblPtr(), mDepthDSV.RawPtr(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(mColorRTV.RawPtr(), RGBA, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(mDepthDSV.RawPtr(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
	}
}