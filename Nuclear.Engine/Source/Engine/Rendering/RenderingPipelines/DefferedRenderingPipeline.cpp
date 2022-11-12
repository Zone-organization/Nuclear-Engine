#include "Engine\Rendering\RenderingPipelines\DefferedRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
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
        void DefferedRenderingPipeline::BeginFrame(FrameRenderData* frame)
        {
            pCurrentFrame = frame;
            pActiveShadingModel = nullptr;
        }
        void DefferedRenderingPipeline::StartStaticShaderModelRendering(ShadingModel* shadingmodel)
        {
            if (!mSkinnedRendering)
            {
                if (pActiveShadingModel != shadingmodel)
                {
                    return StartStaticRendering(shadingmodel);
                }
                return;
            }
            else
            {
                return StartStaticRendering(shadingmodel);
            }
        }
        void DefferedRenderingPipeline::RenderStatic(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
        {
            pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
            pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

           DrawStaticMesh(mesh.mMesh, mesh.mMaterial);
        }

        void DefferedRenderingPipeline::StartSkinnedShaderModelRendering(ShadingModel* shadingmodel)
        {
            if (mSkinnedRendering)
            {
                if (pActiveShadingModel != shadingmodel)
                {
                    return StartSkinnedRendering(shadingmodel);
                }
                return;
            }
            else
            {
                return StartSkinnedRendering(shadingmodel);
            }
        }
        void DefferedRenderingPipeline::RenderSkinned(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
        {
            pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
            pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

            /////////////////////// Animation ////////////////////////////////
            PVoid anim_data;
            Graphics::Context::GetContext()->MapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)anim_data);

            if (mesh.mAnimator != nullptr)
            {
                std::vector<Math::Matrix4> ok;
                ok.reserve(100);

                auto transforms = mesh.mAnimator->GetFinalBoneMatrices();
                for (int i = 0; i < transforms.size(); ++i)
                {
                    ok.push_back(transforms[i]);
                }

                anim_data = memcpy(anim_data, ok.data(), ok.size() * sizeof(Math::Matrix4));
            }
            else
            {

                NUCLEAR_ERROR("[DefferedRenderingPipeline] Rendering SkinnedMeshComponent with no animator...");
                Math::Matrix4 empty(0.0f);
                anim_data = memcpy(anim_data, &empty, sizeof(Math::Matrix4));
            }


            Graphics::Context::GetContext()->UnmapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE);

           DrawSkinnedMesh(mesh.mMesh, mesh.mMaterial);
        }

        void DefferedRenderingPipeline::StartStaticRendering(ShadingModel* shadingmodel)
        {
            pActiveShadingModel = shadingmodel;
            mSkinnedRendering = false;

            //Render To Gbuffer
            Graphics::Context::GetContext()->SetPipelineState(shadingmodel->GetGBufferPipeline());

            std::vector<ITextureView*> RTargets;
            for (auto& i : shadingmodel->mGBuffer.mRenderTargets)
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
        void DefferedRenderingPipeline::StartSkinnedRendering(ShadingModel* shadingmodel)
        {
            pActiveShadingModel = shadingmodel;
            mSkinnedRendering = true;

            //Render To Gbuffer
            Graphics::Context::GetContext()->SetPipelineState(shadingmodel->GetGBufferPipeline());

            std::vector<ITextureView*> RTargets;
            for (auto& i : shadingmodel->mGBuffer.mRenderTargets)
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
    }
}