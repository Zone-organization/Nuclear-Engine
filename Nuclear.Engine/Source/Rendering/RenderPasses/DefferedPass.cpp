#include "Rendering\RenderPasses\DefferedPass.h"
#include <Rendering\FrameRenderData.h>
#include <Assets\Shader.h>
#include <Assets\Texture.h>
#include <Graphics\Context.h>
#include <Graphics\ShaderPipelineVariant.h>
#include <Systems\DebugSystem.h>
#include <Core\Scene.h>
#include <Assets\DefaultMeshes.h>

namespace Nuclear
{
    namespace Rendering
    {
        DefferedPass::DefferedPass()
        {
        }
        void DefferedPass::Update(FrameRenderData* framedata)
        {
            for (auto pipeline : framedata->mUsedDefferedPipelines)
            {
                if (pipeline != nullptr)
                {
                    if (pipeline->mDesc.isDeffered)
                    {
                        auto gbuffer = pipeline->pParent->GetGBuffer();

                        //Apply Lighting
                        Graphics::Context::GetInstance().GetContext()->SetPipelineState(pipeline->mPipeline);
                        Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, framedata->pCamera->GetRenderTarget().GetRTVDblPtr(), nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                        for (int i = 0; i < gbuffer->mRenderTargets.size(); i++)
                        {
                            pipeline->mPipelineSRB->GetVariableByIndex(Diligent::SHADER_TYPE_PIXEL, i)->Set(gbuffer->mRenderTargets.at(i).GetSRV());
                        }

                        //IBL
                        for (int i = 0; i < pipeline->mReflection.mIBLTexturesInfo.size(); i++)
                        {
                            pipeline->mPipelineSRB->GetVariableByIndex(Diligent::SHADER_TYPE_PIXEL, pipeline->mReflection.mIBLTexturesInfo.at(i).mSlot)->Set(pipeline->mReflection.mIBLTexturesInfo.at(i).mTex.pTexture->GetTextureView());
                        }

                        Graphics::Context::GetInstance().GetContext()->CommitShaderResources(pipeline->mPipelineSRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                        Assets::DefaultMeshes::RenderScreenQuad();

                        //Send GBUFFER to DebugSystem
                        if (Core::Scene::GetInstance().GetSystemManager().GetSystem<Systems::DebugSystem>())
                        {
                            for (auto& i : gbuffer->mRenderTargets)
                            {
                                Core::Scene::GetInstance().GetSystemManager().GetSystem<Systems::DebugSystem>()->mRegisteredRTs.push_back(&i);
                            }
                        }
                    }
                }
            }
        }
    }
}