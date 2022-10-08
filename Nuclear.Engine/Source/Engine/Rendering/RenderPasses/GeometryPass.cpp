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

			Uint32 SMCount = 0 , SMFinishedCount = 0;
			ShadingModel* ActiveShadingModel = nullptr;

			//Render Meshes
			for (auto& meshentity : frame->mMeshView)
			{
				auto& mesh = frame->mMeshView.get<Components::MeshComponent>(meshentity);

				if (mesh.mRender)
				{
					if (ActiveShadingModel != mesh.mMaterial->GetShadingModel())
					{
						//first sm rendering
						if (ActiveShadingModel != nullptr)
						{
							pPipeline->FinishShaderModelRendering();
							SMFinishedCount++;
						}

						ActiveShadingModel = mesh.mMaterial->GetShadingModel();

						pPipeline->StartShaderModelRendering(mesh.mMaterial->GetShadingModel());
						SMCount++;
					}

					auto& EntityInfo = frame->pScene->GetRegistry().get<Components::EntityInfoComponent>(meshentity);
					EntityInfo.mTransform.Update();

					pPipeline->Render(mesh, EntityInfo.mTransform.GetWorldMatrix());
				}
			}
			if (SMCount != SMFinishedCount)
			{
				pPipeline->FinishShaderModelRendering();
				SMFinishedCount++;
			}
			//Render Skybox
			if (GetBackground().GetSkybox() != nullptr)
			{
				Graphics::Context::GetContext()->SetRenderTargets(1, frame->mFinalRT.GetRTVDblPtr(), frame->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				GetBackground().GetSkybox()->Render();
			}
		}
	}
}