#include "Engine\Rendering\RenderingPipelines\DefferedRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Components\AnimatorComponent.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\FileSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Systems\DebugSystem.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear
{
	namespace Rendering
	{
		/*void DefferedRenderingPipeline::Initialize(const DefferedRenderingPipelineInitInfo& info)
		{
            SetShadingModelAndCamera(info.shadingModel, info.camera);
            GBufferDesc gbufferdesc;
            gbufferdesc.mRTDescs = info.shadingModel->GetGBufferDesc();
            mGBuffer.Initialize(gbufferdesc);
		}*/

        void DefferedRenderingPipeline::RenderQueue(FrameRenderData* frame, DrawQueue* queue)
        {
            auto shadingmodel = queue->pShadingModel;

            //Render To Gbuffer
            Graphics::Context::GetContext()->SetPipelineState(shadingmodel->GetGBufferPipeline());

            std::vector<ITextureView*> RTargets;
            for (auto& i : shadingmodel->mGBuffer.mRenderTargets)
            {
                RTargets.push_back(i.GetRTV());
            }
            Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), frame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            for (auto& i : RTargets)
            {
                Graphics::Context::GetContext()->ClearRenderTarget(i, nullptr,  RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            }

            Graphics::Context::GetContext()->ClearDepthStencil(frame->mFinalDepthRT.GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            RenderMeshes(frame , queue);

            //Apply Lighting
            Graphics::Context::GetContext()->SetPipelineState(shadingmodel->GetShadersPipeline());
            Graphics::Context::GetContext()->SetRenderTargets(1, frame->mFinalRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            for (int i = 0; i < shadingmodel->mGBuffer.mRenderTargets.size(); i++)
            {
                shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i)->Set(shadingmodel->mGBuffer.mRenderTargets.at(i).GetSRV());
            }

            //IBL
            for (int i = 0; i < shadingmodel->mIBLTexturesInfo.size(); i++)
            {
                shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadingmodel->mIBLTexturesInfo.at(i).mSlot)->Set(shadingmodel->mIBLTexturesInfo.at(i).mTex.GetImage()->mTextureView);
            }

            Graphics::Context::GetContext()->CommitShaderResources(shadingmodel->GetShadersPipelineSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


            Assets::DefaultMeshes::RenderScreenQuad();


            //Send GBUFFER to DebugSystem
            //if (renderer->mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
            //{
            //    for (auto& i : mGBuffer.mRenderTargets)
            //    {
            //        renderer->mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->mRegisteredRTs.push_back(&i);
            //    }
            //}
       //     mGBuffer.DebugIMGUI();
        }

        void DefferedRenderingPipeline::RenderMeshes(FrameRenderData* frame, DrawQueue* queue)
        {
            for (auto& drawcmd : queue->mDrawCommands)
            {
                frame->pCamera->SetModelMatrix(drawcmd.GetTransform());
                frame->pCameraSystemPtr->UpdateBuffer();

                /////////////////////// Animation ////////////////////////////////
                PVoid anim_data;
                Graphics::Context::GetContext()->MapBuffer(frame->pAnimationCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)anim_data);

                if (drawcmd.GetAnimator() != nullptr)
                {
                    std::vector<Math::Matrix4> ok;
                    ok.reserve(100);

                    auto transforms = drawcmd.GetAnimator()->mAnimator->GetFinalBoneMatrices();
                    for (int i = 0; i < transforms.size(); ++i)
                    {
                        ok.push_back(transforms[i]);
                    }

                    anim_data = memcpy(anim_data, ok.data(), ok.size() * sizeof(Math::Matrix4));
                }
                else {
                    Math::Matrix4 empty(0.0f);
                    anim_data = memcpy(anim_data, &empty, sizeof(Math::Matrix4));
                }

                Graphics::Context::GetContext()->UnmapBuffer(frame->pAnimationCB, MAP_WRITE);

                InstantRender(drawcmd.GetMesh(), drawcmd.GetMaterial());
            }
        }
	}
}