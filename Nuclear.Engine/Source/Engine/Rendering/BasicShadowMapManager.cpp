#include "Engine/Rendering/BasicShadowMapManager.h"
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
    namespace Rendering
    {
        BasicShadowMapManager::BasicShadowMapManager()
        {
        }
        void BasicShadowMapManager::Initialize(const ShadowMapInitInfo& info)
        {
            TextureDesc ShadowMapDesc;
            ShadowMapDesc.Name = "BasicShadowSRV";
            ShadowMapDesc.Type = RESOURCE_DIM_TEX_2D;
            ShadowMapDesc.Width = info.mResolution;
            ShadowMapDesc.Height = info.mResolution;
            ShadowMapDesc.MipLevels = 1;
            ShadowMapDesc.Format = info.mFormat;
            ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

            RefCntAutoPtr<ITexture> ShadowMapTex2D;
            Graphics::Context::GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &ShadowMapTex2D);

            mShadowMapSRV = ShadowMapTex2D->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
           // if (initInfo.pComparisonSampler != nullptr)
           //     mShadowMapSRV->SetSampler(initInfo.pComparisonSampler);

            TextureViewDesc ShadowMapDSVDesc;
            ShadowMapDSVDesc.Name = "BasicShadowDSV";
            ShadowMapDSVDesc.ViewType = TEXTURE_VIEW_DEPTH_STENCIL;
            ShadowMapTex2D->CreateView(ShadowMapDSVDesc, &mShadowMapDSV);
        }
        void BasicShadowMapManager::SetForWriting()
        {
        }
    }
}