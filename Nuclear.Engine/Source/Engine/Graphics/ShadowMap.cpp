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
			ShadowMapDesc.Format = TEX_FORMAT_R32_TYPELESS;
			ShadowMapDesc.Usage = USAGE_DEFAULT;
			ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

			if (mType == LightType::DirLight)
			{
				dimension = RESOURCE_DIM_TEX_2D;
				mRTDesc.mType = "DirLight ShadowMap";

			}
			else if (mType == LightType::SpotLight)
			{
				dimension = RESOURCE_DIM_TEX_2D;
				mRTDesc.mType = "SpotLight ShadowMap";

			}
			else if (mType == LightType::PointLight)
			{

				dimension = RESOURCE_DIM_TEX_CUBE;
				ShadowMapDesc.ArraySize = 6;
				mRTDesc.mType = "PointLight ShadowMap";
			}



			ShadowMapDesc.Type = dimension;
			Graphics::Context::GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &ShadowMapTex);

			{
				TextureViewDesc SRVDesc{ "ShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, dimension };
				SRVDesc.Format = TEX_FORMAT_R32_FLOAT;
				ShadowMapTex->CreateView(SRVDesc, &mSRV);
			}
			{
				TextureViewDesc DSVDesc{ "ShadowMap_DSV", TEXTURE_VIEW_DEPTH_STENCIL, dimension };
				DSVDesc.Format = TEX_FORMAT_D32_FLOAT;
				ShadowMapTex->CreateView(DSVDesc, &mRTV);
			}
			mInitialized = true;

		}
	}
}