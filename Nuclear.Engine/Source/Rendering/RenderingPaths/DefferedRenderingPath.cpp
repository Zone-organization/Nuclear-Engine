#include "Rendering\RenderingPaths\DefferedRenderingPath.h"
#include <Graphics/GraphicsModule.h>
#include <Components/MeshComponent.h>
#include <Assets\Shader.h>
#include "Animation/Animator.h"
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Systems\DebugSystem.h>
#include <Rendering\FrameRenderData.h>
#include <Rendering/RenderingModule.h>

namespace Nuclear
{
	namespace Rendering
	{ 

        void DefferedRenderingPath::StartRendering(FrameRenderData* framedata, Graphics::ShaderPipelineVariant* pipeline)
        {
            if (pActivePipeline != pipeline)
            {
                pCurrentFrame = framedata;
                pActivePipeline = pipeline;

                //Render To Gbuffer
                Graphics::GraphicsModule::Get().GetContext()->SetPipelineState(pipeline->mGBufferPipeline);

                std::vector<Diligent::ITextureView*> RTargets;
                for (auto& i : pipeline->pParent->GetGBuffer()->mRenderTargets)
                {
                    RTargets.push_back(i.GetRTV());
                }
                Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), pCurrentFrame->pCamera->GetDepthRT().GetRTV(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                for (auto& i : RTargets)
                {
                    Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(i, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                }

                Graphics::GraphicsModule::Get().GetContext()->ClearDepthStencil(pCurrentFrame->pCamera->GetDepthRT().GetRTV(), Diligent::CLEAR_DEPTH_FLAG, 1.0f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                pCurrentFrame->mUsedDefferedPipelines.push_back(pActivePipeline);
            }
        }
        void DefferedRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
        {
            pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
            Rendering::RenderingModule::Get().UpdateCameraCB(pCurrentFrame->pCamera);

            UpdateAnimationCB(mesh.GetAnimator());

            ////Shadows
            //////////////////////////     TODO    //////////////////////////////////////
            //if (pCurrentFrame->mShadowsEnabled)
            //{
            //    auto& shadowmapsInfo = pActivePipeline->GetShaderAsset()->mShadowMapsInfo;
            //    if (shadowmapsInfo.mDirPos_SMInfo.mType != Assets::ShaderTextureType::Unknown)
            //    {
            //        pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mDirPos_SMInfo.mSlot)->Set(pCurrentFrame->pDirPosShadowMapSRV);
            //    }
            //    if (shadowmapsInfo.mSpot_SMInfo.mType != Assets::ShaderTextureType::Unknown)
            //    {
            //        pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mSpot_SMInfo.mSlot)->Set(pCurrentFrame->pSpotPosShadowMapSRV);
            //    }
            //    if (shadowmapsInfo.mOmniDir_SMInfo.mType != Assets::ShaderTextureType::Unknown)
            //    {
            //        pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mOmniDir_SMInfo.mSlot)->Set(pCurrentFrame->pOmniDirShadowMapSRV);
            //    }
            //}

            DrawMesh(mesh.GetMesh(), mesh.GetMaterial());
        }
    }
}