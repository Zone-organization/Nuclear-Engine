#include <Graphics\RenderTarget.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Graphics/GraphicsModule.h>

namespace Nuclear
{
	namespace Graphics
	{
		using namespace Diligent;

		RenderTarget::RenderTarget()
		{
			mRTV = nullptr;
			mSRV = nullptr;
		}
		RenderTarget::~RenderTarget()
		{

		}
		void RenderTarget::Create(const RenderTargetDesc& Desc)
		{
			mDesc = Desc;
			CreateViews();
		}

		void RenderTarget::Release()
		{
			if (mRTV.RawPtr() != nullptr)
			{
				mRTV.Release();
			}
			if (mSRV.RawPtr() != nullptr)
			{
				mSRV.Release();
			}
			mRTV = nullptr;
			mSRV = nullptr;
		}

		void RenderTarget::Resize(const Math::Vector2ui& dimensions)
		{
			mDesc.mDimensions = dimensions;
			Create(mDesc);
		}

		Math::Vector2ui RenderTarget::GetDimensions() const
		{
			return mDesc.mDimensions;
		}

		ITextureView* RenderTarget::GetSRV()
		{
			return mSRV.RawPtr();
		}

		ITextureView* RenderTarget::GetRTV()
		{
			return mRTV.RawPtr();
		}
		ITextureView** RenderTarget::GetRTVDblPtr()
		{
			return mRTV.RawDblPtr();
		}
		RenderTargetDesc RenderTarget::GetDesc() const
		{
			return mDesc;
		}
		void RenderTarget::CreateViews()
		{
			Release();

			TextureDesc TexDesc;
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = mDesc.mDimensions.x;
			TexDesc.Height = mDesc.mDimensions.y;

			if (mDesc.DepthTexFormat != TEX_FORMAT_UNKNOWN)
			{
				//Create Depth Texture
				RefCntAutoPtr<ITexture> pRTDepth;
				TexDesc.Format = mDesc.DepthTexFormat;
				TexDesc.ClearValue.Format = TexDesc.Format;
				TexDesc.ClearValue.DepthStencil.Depth = 1;
				TexDesc.ClearValue.DepthStencil.Stencil = 0;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &pRTDepth);
				mRTV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);
			}
			else {

				RefCntAutoPtr<ITexture> pRTColor;

				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Format = mDesc.ColorTexFormat;
				TexDesc.MipLevels = 1;
				TexDesc.ClearValue = mDesc.ClearValue;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &pRTColor);
				mRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);
				mSRV = pRTColor->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
			}
		}
	}
}