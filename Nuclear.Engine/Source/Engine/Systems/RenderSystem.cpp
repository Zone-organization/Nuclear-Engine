#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/EntityInfoComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Scene.h>
#include <Engine.h>
#include <cstring>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Systems\LightingSystem.h>

namespace Nuclear
{
	namespace Systems
	{
		RenderSystem::RenderSystem()
		{
			mActiveRenderingPipeline = nullptr;
		}
		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::AddRenderingPipeline(Rendering::RenderingPipeline* Pipeline)
		{
			if (!Pipeline)
			{
				NUCLEAR_ERROR("[RenderSystem] Pipeline is invalid!");
				return;
			}
			mRenderingPipelines[Pipeline->GetID()] = Pipeline;

			if (mActiveRenderingPipeline == nullptr)
				mActiveRenderingPipeline =	mRenderingPipelines[Pipeline->GetID()];
		}

		void RenderSystem::SetActiveRenderingPipeline(Uint32 PipelineID)
		{
			if (mActiveRenderingPipeline)
			{
				if (mActiveRenderingPipeline->GetID() == PipelineID)
				{
					return;
				}
			}

			auto it = mRenderingPipelines.find(PipelineID);
			if (it != mRenderingPipelines.end())
			{
				mActiveRenderingPipeline = it->second;
				return;
			}
			NUCLEAR_ERROR("[RenderSystem] Pipeline ID '{0}' is not found, while setting it active", Utilities::int_to_hex<Uint32>(PipelineID));
		}

		void RenderSystem::CreateMaterialForAllPipelines(Assets::Material* material)
		{
			if (!material)
				return;

			for (auto it : mRenderingPipelines)
			{
				material->CreateInstance(it.second->GetShadingModel());
			}
		}

		void RenderSystem::CreateMaterial(Assets::Material* material, Uint32 PipelineID)
		{
			if (!material)
				return;

			auto it = mRenderingPipelines.find(PipelineID);
			if (it != mRenderingPipelines.end())
			{
				material->CreateInstance(it->second->GetShadingModel());
				return;
			}

			NUCLEAR_ERROR("[RenderSystem] Pipeline ID '{0}' is not found, while creating material instance from it." , Utilities::int_to_hex<Uint32>(PipelineID));
		}

		bool RenderSystem::NeedsBaking()
		{
			if (mStatus.BakeLighting || mStatus.BakePipelines)
			{
				return true;
			}

			return false;
		}

		void RenderSystem::Bake(Uint32 RTWidth, Uint32 RTHeight, bool AllPipelines)
		{
		
			mCameraSystemPtr = mScene->GetSystemManager().GetSystem<CameraSystem>();
			mLightingSystemPtr = mScene->GetSystemManager().GetSystem<LightingSystem>();


			//mLightingSystem.UpdateBuffer(Math::Vector4(mCameraSystem.GetMainCamera()->GetPosition(), 1.0f));

			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_Animation";
			CBDesc.Size = sizeof(Math::Matrix4) * 100;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mAnimationCB);

			Rendering::RenderingPipelineBakingDesc bakedesc;

			//auto lightsystemdesc = mLightingSystemPtr->GetDesc();
			bakedesc.mRTWidth = RTWidth;
			bakedesc.mRTHeight = RTHeight;

			bakedesc.mShadingModelDesc.DirLights = static_cast<Uint32>(mLightingSystemPtr->GetDirLightsNum());
			bakedesc.mShadingModelDesc.SpotLights = static_cast<Uint32>(mLightingSystemPtr->GetSpotLightsNum());
			bakedesc.mShadingModelDesc.PointLights = static_cast<Uint32>(mLightingSystemPtr->GetPointLightsNum());
			bakedesc.mShadingModelDesc.CameraBufferPtr = mCameraSystemPtr->GetCameraCB();
			bakedesc.mShadingModelDesc.LightsBufferPtr = mLightingSystemPtr->GetLightCB();
			bakedesc.mShadingModelDesc.pShadowPass = mLightingSystemPtr->GetShadowPass();

			bakedesc.mShadingModelDesc.AnimationBufferPtr = mAnimationCB;

			//if (lightsystemdesc.DisableShadows)
			//{
			//	
			//}
			//else {
			//	bakedesc.mShadingModelDesc.
			//}
			if(AllPipelines)
			{ 
				for (auto it : mRenderingPipelines)
				{


					it.second->Bake(bakedesc);
				}
			}
			else
			{
				assert(false);
			}
		}
		void RenderSystem::ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight)
		{
			mActiveRenderingPipeline->ResizeRenderTargets(RTWidth, RTHeight);
		}
		Rendering::RenderingPipeline* RenderSystem::GetActivePipeline()
		{
			return mActiveRenderingPipeline;
		}

		void RenderSystem::AddRenderPass(Rendering::RenderPass* pass)
		{
			//pass->Initialize();
			mRenderPasses.push_back(pass);
		}
		
		IBuffer* RenderSystem::GetAnimationCB()
		{
			return mAnimationCB;
		}

		CameraSystem* RenderSystem::GetCameraSystem()
		{
			return mCameraSystemPtr.get();
		}

		LightingSystem* RenderSystem::GetLightingSystem()
		{
			return mLightingSystemPtr.get();
		}

		Rendering::Background& RenderSystem::GetBackground()
		{
			return mBackground;
		}

		void RenderSystem::Update(ECS::TimeDelta dt)
		{
			//Render Scene from each active camera perspective
			//for (auto Camera : mCameraSystem.ActiveCameras)
			{			
					
				//Update light buffer
				auto Camera = mCameraSystemPtr->GetMainCamera();
				mLightingSystemPtr->UpdateBuffer(Math::Vector4(Camera->GetPosition(), 1.0f));

				GetActivePipeline()->StartRendering(this);
			}

			//Render pipeline render targets
			GetActivePipeline()->SetFinalPipelineState();

			//Render Main camera view to screen
			auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
			auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();
			Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(RTV, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Assets::DefaultMeshes::RenderScreenQuad();
		}
	}
}