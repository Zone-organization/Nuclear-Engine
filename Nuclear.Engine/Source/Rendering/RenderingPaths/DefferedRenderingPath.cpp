#include "Rendering\RenderingPaths\DefferedRenderingPath.h"
#include <Graphics\Context.h>
#include <Systems\RenderSystem.h>
#include <Components/MeshComponent.h>
#include <Assets\Shader.h>
#include "Animation/Animator.h"
#include <Utilities/Logger.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Platform\FileSystem.h>
#include <Systems\CameraSystem.h>
#include <Systems\DebugSystem.h>

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
                Graphics::Context::GetInstance().GetContext()->SetPipelineState(pipeline->GetGBufferPipeline());

                std::vector<ITextureView*> RTargets;
                for (auto& i : pipeline->GetParentPipeline()->GetGBuffer()->mRenderTargets)
                {
                    RTargets.push_back(i.GetRTV());
                }
                Graphics::Context::GetInstance().GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), pCurrentFrame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                for (auto& i : RTargets)
                {
                    Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(i, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                }

                Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(pCurrentFrame->mFinalDepthRT.GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                pCurrentFrame->mUsedDefferedPipelines.push_back(pActivePipeline);
            }
        }
        void DefferedRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
        {
            pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
            pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

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