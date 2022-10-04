#include "Engine\Rendering\RenderPasses\GeometryPass.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
	namespace Rendering
	{
		GeometryPass::GeometryPass()
		{
		}
		void GeometryPass::Initialize(RenderingPipeline* pipeline)
		{
		}
		void GeometryPass::Update(FrameRenderData* framedata)
		{	
			////clean Render targets
			//auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
			//auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();
			//Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			//Graphics::Context::GetContext()->ClearRenderTarget(RTV, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			//Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

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