#include "Engine\Rendering\RenderingPaths\DefferedRenderingPath.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Assets\Shader.h>
#include "Engine/Animation/Animator.h"
#include <Core/Logger.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\FileSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Systems\DebugSystem.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear
{
	namespace Rendering
	{ 

        void DefferedRenderingPath::StartRendering(Graphics::ShaderPipelineVariant* pipeline)
        {
            pActivePipeline = pipeline;

            //Render To Gbuffer
            Graphics::Context::GetContext()->SetPipelineState(pipeline->GetGBufferPipeline());

            std::vector<ITextureView*> RTargets;
            for (auto& i : pipeline->mGBuffer.mRenderTargets)
            {
                RTargets.push_back(i.GetRTV());
            }
            Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), pCurrentFrame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            for (auto& i : RTargets)
            {
                Graphics::Context::GetContext()->ClearRenderTarget(i, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            }

            Graphics::Context::GetContext()->ClearDepthStencil(pCurrentFrame->mFinalDepthRT.GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }
        void DefferedRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
        {
            pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
            pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

            UpdateAnimationCB(mesh.mAnimator);

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

            DrawMesh(mesh.mMesh, mesh.mMaterial);
        }
    }
}