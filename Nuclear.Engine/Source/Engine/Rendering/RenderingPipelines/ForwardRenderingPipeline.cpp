#include "Engine/Rendering/RenderingPipelines/ForwardRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Components\AnimatorComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear
{
	namespace Rendering
	{
		void ForwardRenderingPipeline::RenderQueue(FrameRenderData* frame, DrawQueue* queue)
		{
			auto shadingmodel = queue->pShadingModel;
			//Render Meshes
			Graphics::Context::GetContext()->SetPipelineState(shadingmodel->GetShadersPipeline());

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

				////////////////////////      IBL      ///////////////////////////
				for (int i = 0; i < shadingmodel->mIBLTexturesInfo.size(); i++)
				{
					shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadingmodel->mIBLTexturesInfo.at(i).mSlot)->Set(shadingmodel->mIBLTexturesInfo.at(i).mTex.GetImage()->mTextureView);
				}

				//Shadows
				////////////////////////     TODO    //////////////////////////////////////
				if (frame->mShadowsEnabled)
				{
					if (shadingmodel->mDirPos_ShadowmapInfo.mType != Assets::ShaderTextureType::Unknown)
					{
						shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadingmodel->mDirPos_ShadowmapInfo.mSlot)->Set(frame->pDirPosShadowMapSRV);
					}
					if (shadingmodel->mSpot_ShadowmapInfo.mType != Assets::ShaderTextureType::Unknown)
					{
						shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadingmodel->mSpot_ShadowmapInfo.mSlot)->Set(frame->pSpotPosShadowMapSRV);
					}
					if (shadingmodel->mOmniDir_ShadowmapInfo.mType != Assets::ShaderTextureType::Unknown)
					{
						shadingmodel->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadingmodel->mOmniDir_ShadowmapInfo.mSlot)->Set(frame->pOmniDirShadowMapSRV);
					}
				}

				InstantRender(drawcmd.GetMesh(), drawcmd.GetMaterial());
			}
		}
	}
}