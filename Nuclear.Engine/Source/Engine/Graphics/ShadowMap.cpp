#include <Engine\Graphics\ShadowMap.h>
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Graphics
	{
		ShadowMap::ShadowMap(const LightType& lighttype)
			: mType(lighttype)
		{
			mInitialized = false;
		}
		ShadowMap::~ShadowMap()
		{
		}
		bool ShadowMap::isInitialized()
		{
			return mInitialized;
		}
		ShadowMapDesc ShadowMap::GetDesc() const
		{
			return mDesc;
		}
		ITextureView* ShadowMap::GetDSV()
		{
			return mDSV.RawPtr();
		}
		ITextureView* ShadowMap::GetSRV()
		{
			return mSRV.RawPtr();
		}
		void ShadowMap::Initialize(const ShadowMapDesc& Desc)
		{
			if (mType == LightType::SpotLight)
			{
				TextureDesc ShadowMapDesc;
				ShadowMapDesc.Name = "Spotlight_ShadowMap";
				ShadowMapDesc.Type = RESOURCE_DIM_TEX_2D;
				ShadowMapDesc.Width = mDesc.mResolution;
				ShadowMapDesc.Height = mDesc.mResolution;
				ShadowMapDesc.MipLevels = 1;
				ShadowMapDesc.SampleCount = 1;
				ShadowMapDesc.Format = TEX_FORMAT_R24G8_TYPELESS;
				ShadowMapDesc.Usage = USAGE_DEFAULT;
				ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

				RefCntAutoPtr<ITexture> ShadowMapTex2D;
				Graphics::Context::GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &ShadowMapTex2D);

				{
					TextureViewDesc SRVDesc{ "ShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, RESOURCE_DIM_TEX_2D };
					SRVDesc.Format = TEX_FORMAT_R24_UNORM_X8_TYPELESS;
					ShadowMapTex2D->CreateView(SRVDesc, &mSRV);
				}
				{
					TextureViewDesc DSVDesc{ "ShadowMap_DSV", TEXTURE_VIEW_DEPTH_STENCIL, RESOURCE_DIM_TEX_2D };
					DSVDesc.Format = TEX_FORMAT_D24_UNORM_S8_UINT;
					ShadowMapTex2D->CreateView(DSVDesc, &mDSV);
				}
				mInitialized = true;
			}
		}
	}
}