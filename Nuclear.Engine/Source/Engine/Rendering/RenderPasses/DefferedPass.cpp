#include "Engine\Rendering\RenderPasses\DefferedPass.h"
#include <Engine\Rendering\FrameRenderData.h>
#include <Engine\Assets\Shader.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Systems\DebugSystem.h>
#include <Engine\Assets\Scene.h>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
    namespace Rendering
    {
        DefferedPass::DefferedPass()
        {
        }
        void DefferedPass::Update(FrameRenderData* framedata)
        {
            for (auto pipeline : framedata->mUsedPipelines)
            {
                if (pipeline != nullptr)
                {
                    if (pipeline->isDeffered())
                    {
                        //Apply Lighting
                        Graphics::Context::GetContext()->SetPipelineState(pipeline->GetShadersPipeline());
                        Graphics::Context::GetContext()->SetRenderTargets(1, framedata->mFinalRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                        for (int i = 0; i < pipeline->mGBuffer.mRenderTargets.size(); i++)
                        {
                            pipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i)->Set(pipeline->mGBuffer.mRenderTargets.at(i).GetSRV());
                        }

                        //IBL
                        for (int i = 0; i < pipeline->GetShaderAsset()->mIBLTexturesInfo.size(); i++)
                        {
                            pipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, pipeline->GetShaderAsset()->mIBLTexturesInfo.at(i).mSlot)->Set(pipeline->GetShaderAsset()->mIBLTexturesInfo.at(i).mTex.GetImage()->mTextureView);
                        }

                        Graphics::Context::GetContext()->CommitShaderResources(pipeline->GetShadersPipelineSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                        Assets::DefaultMeshes::RenderScreenQuad();

                        pipeline = nullptr;

                        //Send GBUFFER to DebugSystem
                        if (framedata->pScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
                        {
                            for (auto& i : pipeline->mGBuffer.mRenderTargets)
                            {
                                framedata->pScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->mRegisteredRTs.push_back(&i);
                            }
                        }
                    }
                }
            }
        }
    }
}