#include "Rendering\RenderPasses\GeometryPass.h"
#include <Rendering\FrameRenderData.h>
#include <Components\EntityInfoComponent.h>
#include <Core\Scene.h>
#include <Assets\Shader.h>
#include <Assets\Material.h>

namespace Nuclear
{
	namespace Rendering
	{
		GeometryPass::GeometryPass()
		{
		}

		void GeometryPass::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{


		}

		void GeometryPass::Update(FrameRenderData* frame)
		{
			bool SkinnedRendering = false;
			frame->mUsedDefferedPipelines.clear();

			//if()
			mForwardPath.Reset();
			mDefferedPath.Reset();

			//Render Meshes
			for (auto& meshentity : frame->mMeshView)
			{
				auto& mesh = frame->mMeshView.get<Components::MeshComponent>(meshentity);

				mesh.Update();

				if (mesh.GetRenderQueue() > 0)
				{			
					auto pipelinevariant = mesh.GetRenderingVariant();

					if (pipelinevariant->mDesc.isDeffered)
					{
						pRenderingPath = &mDefferedPath;
					}
					else {
						pRenderingPath = &mForwardPath;
					}
					pRenderingPath->StartRendering(frame, pipelinevariant);

					auto& EntityInfo = Core::Scene::Get().GetRegistry().get<Components::EntityInfoComponent>(meshentity);
					EntityInfo.mTransform.Update();

					pRenderingPath->Render(mesh, EntityInfo.mTransform.GetWorldMatrix());
				}

			}
		}		
	}
}