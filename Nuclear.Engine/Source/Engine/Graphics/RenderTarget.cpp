#include <Engine\Graphics\RenderTarget.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Engine\Graphics\Context.h>

namespace Nuclear
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
			mDesc = Desc;
			CreateViews();
		}

		void RenderTarget::Resize(Uint32 width, Uint32 height)
		{
			mDesc.Width = width;
			mDesc.Height = height;
			Create(mDesc);
		}

		Uint32 RenderTarget::GetWidth() const
		{
			return mDesc.Width;
		}

		Uint32 RenderTarget::GetHeight() const
		{
			return mDesc.Height;
		}

		ITextureView* RenderTarget::GetShaderRTV()
		{
			return mShaderRTV.RawPtr();
		}

		ITextureView* RenderTarget::GetMainRTV()
		{
			return mRTV.RawPtr();
		}
		ITextureView** RenderTarget::GetMainRTVDblPtr()
		{
			return mRTV.RawDblPtr();
		}
		RenderTargetDesc RenderTarget::GetDesc() const
		{
			return mDesc;
		}
		void RenderTarget::CreateViews()
		{
			if (mRTV.RawPtr() != nullptr)
			{
				mRTV.Release();
			}
			if (mShaderRTV.RawPtr() != nullptr)
			{
				mShaderRTV.Release();
			}

			TextureDesc TexDesc;
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = mDesc.Width;
			TexDesc.Height = mDesc.Height;

			if (mDesc.DepthTexFormat != TEX_FORMAT_UNKNOWN)
			{
				//Create Depth Texture
				RefCntAutoPtr<ITexture> pRTDepth;
				TexDesc.Format = mDesc.DepthTexFormat;
				TexDesc.ClearValue.Format = TexDesc.Format;
				TexDesc.ClearValue.DepthStencil.Depth = 1;
				TexDesc.ClearValue.DepthStencil.Stencil = 0;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

				Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &pRTDepth);
				mRTV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);
			}
			else {

				RefCntAutoPtr<ITexture> pRTColor;

				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Format = mDesc.ColorTexFormat;
				TexDesc.MipLevels = 1;
				TexDesc.ClearValue = mDesc.ClearValue;

				Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &pRTColor);
				mRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);
				mShaderRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
			}
		}
		RenderTargetDesc::RenderTargetDesc()
		{
			mName = "UnNamed";
			mType = "UnNamed";
		}
		RenderTargetDesc::RenderTargetDesc(TEXTURE_FORMAT colorTexFormat, TEXTURE_FORMAT mDepthTexFormat, std::string name)
		{
			ColorTexFormat = colorTexFormat;
			DepthTexFormat = mDepthTexFormat;
			mName = name;
			mType = "UnNamed";
		}
	}
}