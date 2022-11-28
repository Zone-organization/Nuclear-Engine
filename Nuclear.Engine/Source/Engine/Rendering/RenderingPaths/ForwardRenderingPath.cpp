#include "Engine/Rendering/RenderingPaths/ForwardRenderingPath.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Components/MeshComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Assets\Scene.h>
#include <Engine\Assets\Shader.h>
#include <Core/Logger.h>
#include <Engine\Graphics\ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Rendering
	{
		void ForwardRenderingPath::StartRendering(Graphics::ShaderPipelineVariant* pipeline)
		{
			if (pActivePipeline != pipeline)
			{
				pActivePipeline = pipeline;
				Graphics::Context::GetContext()->SetPipelineState(pipeline->GetMainPipeline());
				Graphics::Context::GetContext()->SetRenderTargets(1, pCurrentFrame->mFinalRT.GetRTVDblPtr(), pCurrentFrame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			}
		}
			
		void ForwardRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
		{
			pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
			pCurrentFrame->pCameraSystemPtr->UpdateBuffer();

			UpdateAnimationCB(mesh.GetAnimator());

			////////////////////////      IBL      ///////////////////////////
			for (auto& i : pActivePipeline->GetReflection().mIBLTexturesInfo)
			{
				pActivePipeline->GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i.mSlot)->Set(i.mTex.GetImage()->mTextureView);
			}

			//Shadows
			////////////////////////     TODO    //////////////////////////////////////
			if (pCurrentFrame->mShadowsEnabled && pActivePipeline->isShadowed())
			{
				auto& shadowmapsInfo = pActivePipeline->GetReflection().mShadowMapsInfo;
				if (shadowmapsInfo.mDirPos_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mDirPos_SMInfo.mSlot)->Set(pCurrentFrame->pDirPosShadowMapSRV);
				}
				if (shadowmapsInfo.mSpot_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mSpot_SMInfo.mSlot)->Set(pCurrentFrame->pSpotPosShadowMapSRV);
				}
				if (shadowmapsInfo.mOmniDir_SMInfo.mType != Assets::ShaderTextureType::Unknown)
				{
					pActivePipeline->GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, shadowmapsInfo.mOmniDir_SMInfo.mSlot)->Set(pCurrentFrame->pOmniDirShadowMapSRV);
				}
			}

			DrawMesh(mesh.GetMesh(), mesh.GetMaterial());
		}
	}
}