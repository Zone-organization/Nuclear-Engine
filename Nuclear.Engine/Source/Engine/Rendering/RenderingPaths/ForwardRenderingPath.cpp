#include "Engine/Rendering/RenderingPaths/ForwardRenderingPath.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Components/MeshComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Assets\Scene.h>
#include <Core/Logger.h>
#include <Engine\Rendering\ShaderPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{
		void ForwardRenderingPath::StartRendering(ShaderPipeline* pipeline)
		{
			pActivePipeline = pipeline;
			Graphics::Context::GetContext()->SetPipelineState(pipeline->GetShadersPipeline());
			Graphics::Context::GetContext()->SetRenderTargets(1, pCurrentFrame->mFinalRT.GetRTVDblPtr(), pCurrentFrame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
		void ForwardRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
		{
			pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
			pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

			UpdateAnimationCB(mesh.mAnimator);

			////////////////////////      IBL      ///////////////////////////
			for (int i = 0; i < pActivePipeline->GetShaderAsset()->mIBLTexturesInfo.size(); i++)
			{
				pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, pActivePipeline->GetShaderAsset()->mIBLTexturesInfo.at(i).mSlot)->Set(pActivePipeline->GetShaderAsset()->mIBLTexturesInfo.at(i).mTex.GetImage()->mTextureView);
			}

			//Shadows
			////////////////////////     TODO    //////////////////////////////////////
			if (pCurrentFrame->mShadowsEnabled)
			{
				auto& shadowmapsInfo = pActivePipeline->GetShaderAsset()->mShadowMapsInfo;
				if (shadowmapsInfo.mDirPos_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mDirPos_SMInfo.mSlot)->Set(pCurrentFrame->pDirPosShadowMapSRV);
				}
				if (shadowmapsInfo.mSpot_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mSpot_SMInfo.mSlot)->Set(pCurrentFrame->pSpotPosShadowMapSRV);
				}
				if (shadowmapsInfo.mOmniDir_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mOmniDir_SMInfo.mSlot)->Set(pCurrentFrame->pOmniDirShadowMapSRV);
				}
			}

			DrawMesh(mesh.mMesh, mesh.mMaterial);
		}
	}
}