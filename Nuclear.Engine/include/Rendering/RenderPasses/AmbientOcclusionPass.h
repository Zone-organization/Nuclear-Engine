#pragma once
#include <Rendering\RenderPass.h>
#include <Graphics\RenderTarget.h>

namespace Nuclear
{
    namespace Assets
    {
        class Shader;
    }
    namespace Rendering
    {
        struct FrameRenderData;


        //Currently uses SSAO
        //Todo: ASSAO & XeGTAO...
        class NEAPI AmbientOcclusionPass : public RenderPass
        {
        public:

            void Bake(Uint32 RTWidth, Uint32 RTHeight);

            void Update(FrameRenderData* framedata) override;

            void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight) override;

        protected:
            Assets::Shader* pSSAO_ExtractShader;
            Assets::Shader* pSSAO_BlurShader;

            Diligent::RefCntAutoPtr<Diligent::IBuffer> pSSAO_ExtractCB;
            Diligent::RefCntAutoPtr<Diligent::ITextureView> pNoiseTexView;

            Graphics::RenderTarget mSSAO_RT;
        };
    }
}