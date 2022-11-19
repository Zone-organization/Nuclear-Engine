#include "Engine\Rendering\RenderPasses\GeometryPass.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Rendering\FrameRenderData.h>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Assets\Scene.h>
#include <Engine\Assets\Shader.h>
#include <Engine\Assets\Material.h>
#include <Engine\Systems\CameraSystem.h>
#include <Core\Logger.h>
#include <Engine.h>

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

		Rendering::Background& GeometryPass::GetBackground()
		{
			return mBackground;
		}

		void GeometryPass::Update(FrameRenderData* frame)
		{
			bool SkinnedRendering = false;
			frame->mUsedPipelines.clear();
			//Render Meshes
			for (auto& meshentity : frame->mMeshView)
			{
				auto& mesh = frame->mMeshView.get<Components::MeshComponent>(meshentity);

				if (mesh.GetRenderQueue() != -1)
				{			
					pRenderingPath->StartRendering(mesh.GetRenderingVariant());

					auto& EntityInfo = frame->pScene->GetRegistry().get<Components::EntityInfoComponent>(meshentity);
					EntityInfo.mTransform.Update();

					pRenderingPath->Render(mesh, EntityInfo.mTransform.GetWorldMatrix());
					frame->mUsedPipelines.push_back(mesh.GetRenderingVariant());
				}

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