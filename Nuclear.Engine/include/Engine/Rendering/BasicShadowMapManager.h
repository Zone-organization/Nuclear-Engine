#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/RenderTarget.h>
#include <Core/Math/Math.h>
#include <string>
#include <Diligent/Common/interface/AdvancedMath.hpp>
#include "Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Diligent/Graphics/GraphicsEngine/interface/Texture.h"
#include "Diligent/Graphics/GraphicsEngine/interface/TextureView.h"
#include "Diligent/Common/interface/RefCntAutoPtr.hpp"

namespace Nuclear
{
    namespace Rendering
    {
        //Directional & Spotlights
        //Single shadow map

        struct ShadowMapInitInfo {
            Uint32 mResolution = 1024;
            TEXTURE_FORMAT mFormat = TEX_FORMAT_D16_UNORM;
        };

        class BasicShadowMapManager
        {
        public:
            BasicShadowMapManager();

            void Initialize(const ShadowMapInitInfo& info);

            void SetForWriting();
        private:
            RefCntAutoPtr<ITextureView> mShadowMapSRV;
            RefCntAutoPtr<ITextureView> mShadowMapDSV;
        };
    }
}