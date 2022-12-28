#include "Rendering/RenderingPaths/ForwardRenderingPath.h"
#include <Graphics\Context.h>
#include <Components/MeshComponent.h>
#include "Animation/Animator.h"
#include <Assets\Shader.h>
#include <Assets\Image.h>
#include <Graphics\ShaderPipelineVariant.h>
#include <Rendering/RenderingEngine.h>
#include <Rendering\FrameRenderData.h>

namespace Nuclear
{
	namespace Rendering
	{
		void ForwardRenderingPath::StartRendering(FrameRenderData* frame,Graphics::ShaderPipelineVariant* pipeline)
		{
			if (pActivePipeline != pipeline)
			{
				pCurrentFrame = frame;
				pActivePipeline = pipeline;
				Graphics::Context::GetInstance().GetContext()->SetPipelineState(pipeline->GetMainPipeline());
				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, pCurrentFrame->pCamera->GetRenderTarget().GetRTVDblPtr(), pCurrentFrame->pCamera->GetDepthRenderTarget().GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			}
		}
			
		void ForwardRenderingPath::Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix)
		{
			pCurrentFrame->pCamera->SetModelMatrix(modelmatrix);
			Rendering::RenderingEngine::GetInstance().UpdateCameraCB(pCurrentFrame->pCamera);

			UpdateAnimationCB(mesh.GetAnimator());

			////////////////////////      IBL      ///////////////////////////
			for (auto& i : pActivePipeline->GetReflection().mIBLTexturesInfo)
			{
				pActivePipeline->GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i.mSlot)->Set(i.mTex.GetImage()->GetTextureView());
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