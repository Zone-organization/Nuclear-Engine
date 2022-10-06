#include "Engine\Rendering\RenderPasses\GeometryPass.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Rendering\FrameRenderData.h>

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
		void GeometryPass::Update(FrameRenderData* framedata)
		{	
			//Render Meshes
			for (auto& drawqueue : framedata->mSubmittedDraws)
			{
				pPipeline->RenderQueue(framedata, &drawqueue.second);
			}

			//Render Skybox
			if (GetBackground().GetSkybox() != nullptr)
			{
				Graphics::Context::GetContext()->SetRenderTargets(1, framedata->mFinalRT.GetRTVDblPtr(), framedata->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				GetBackground().GetSkybox()->Render();
			}

			////Render Scene from each active camera perspective
			////for (auto Camera : mCameraSystem.ActiveCameras)
			//{

			//	//Update light buffer
			//	auto Camera = mCameraSystemPtr->GetMainCamera();
			//	mLightingSystemPtr->UpdateBuffer(Math::Vector4(Camera->GetPosition(), 1.0f));

			//	GetActivePipeline()->StartRendering(this);
			//}

			////Render pipeline render targets
			//GetActivePipeline()->SetFinalPipelineState();

			////Render Main camera view to screen
			//Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			//Assets::DefaultMeshes::RenderScreenQuad();
		}
	}
}