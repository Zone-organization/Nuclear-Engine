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
			RefCntAutoPtr<ITexture> ShadowMapTex;
			RESOURCE_DIMENSION dimension = RESOURCE_DIM_UNDEFINED;
			TextureDesc ShadowMapDesc;
			ShadowMapDesc.Name = "ShadowMap";
			ShadowMapDesc.Width = mDesc.mResolution;
			ShadowMapDesc.Height = mDesc.mResolution;
			ShadowMapDesc.MipLevels = 1;
			ShadowMapDesc.SampleCount = 1;
			ShadowMapDesc.Format = TEX_FORMAT_R24G8_TYPELESS;
			ShadowMapDesc.Usage = USAGE_DEFAULT;
			ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

			if (mType == LightType::SpotLight)
			{
				dimension = RESOURCE_DIM_TEX_2D;
				mInitialized = true;
			}
			else if (mType == LightType::PointLight)
			{

				dimension = RESOURCE_DIM_TEX_CUBE;
				ShadowMapDesc.ArraySize = 6;

			}



			ShadowMapDesc.Type = dimension;
			Graphics::Context::GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &ShadowMapTex);

			{
				TextureViewDesc SRVDesc{ "ShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, dimension };
				SRVDesc.Format = TEX_FORMAT_R24_UNORM_X8_TYPELESS;
				ShadowMapTex->CreateView(SRVDesc, &mSRV);
			}
			{
				TextureViewDesc DSVDesc{ "ShadowMap_DSV", TEXTURE_VIEW_DEPTH_STENCIL, dimension };
				DSVDesc.Format = TEX_FORMAT_D24_UNORM_S8_UINT;
				ShadowMapTex->CreateView(DSVDesc, &mDSV);
			}
			mInitialized = true;

		}
	}
}