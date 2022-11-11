#include "Engine\Rendering\RenderPasses\GeometryPass.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Rendering\FrameRenderData.h>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Assets\Scene.h>
#include <Engine\Assets\Material.h>

namespace Nuclear
{
	namespace Rendering
	{
		GeometryPass::GeometryPass()
		{
		}
		void GeometryPass::Initialize(RenderingPipeline* pipeline)
		{
			pPipeline = pipeline;
		}
		void GeometryPass::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
		

		}
		Rendering::Background& GeometryPass::GetBackground()
		{
			return mBackground;
		}
		void GeometryPass::Update(FrameRenderData* frame)
		{	
			pPipeline->BeginFrame(frame);

			bool SkinnedRendering = false;

			mStatus = GeometryPassStatus::Idle;

			//Render Meshes
			for (auto& meshentity : frame->mMeshView)
			{
				auto& mesh = frame->mMeshView.get<Components::MeshComponent>(meshentity);

				if (mesh.mRender)
				{
					if (!mesh.mAnimator)
					{
						mStatus = GeometryPassStatus::StaticMeshesRendering;

						pPipeline->StartStaticShaderModelRendering(mesh.mMaterial->GetShadingModel());

						auto& EntityInfo = frame->pScene->GetRegistry().get<Components::EntityInfoComponent>(meshentity);
						EntityInfo.mTransform.Update();

						pPipeline->RenderStatic(mesh, EntityInfo.mTransform.GetWorldMatrix());
					}
					else
					{						
						mStatus = GeometryPassStatus::SkinnedMeshesRendering;

						pPipeline->StartSkinnedShaderModelRendering(mesh.mMaterial->GetShadingModel());

						auto& EntityInfo = frame->pScene->GetRegistry().get<Components::EntityInfoComponent>(meshentity);
						EntityInfo.mTransform.Update();

						pPipeline->RenderSkinned(mesh, EntityInfo.mTransform.GetWorldMatrix());

					}
				}

			}

			pPipeline->FinishAllRendering();

			//Render Skybox
			if (GetBackground().GetSkybox() != nullptr)
			{
				Graphics::Context::GetContext()->SetRenderTargets(1, frame->mFinalRT.GetRTVDblPtr(), frame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				GetBackground().GetSkybox()->Render();
			}

			mStatus = GeometryPassStatus::Idle;

		}
	}
}