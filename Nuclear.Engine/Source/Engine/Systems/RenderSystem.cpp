#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/EntityInfoComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\ECS\Scene.h>
#include <Engine.h>
#include <cstring>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
	namespace Systems
	{
		RenderSystem::RenderSystem(Graphics::Camera* startingcamera)
			:mCameraSystem(startingcamera)
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
				Log.Error("[RenderSystem] Pipeline is invalid!\n");
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
			Log.Error("[RenderSystem] Pipeline ID(" + Utilities::int_to_hex<Uint32>(PipelineID)+ ") is not found, while setting it active.\n");
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

			Log.Error("[RenderSystem] Pipeline ID(" + Utilities::int_to_hex<Uint32>(PipelineID) + ") is not found, while creating material instance from it.\n");
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
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);

				mLightingSystem.DirLights.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				mLightingSystem.SpotLights.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
				mLightingSystem.PointLights.push_back(&PointLight);
			}

			mLightingSystem.BakeBuffer();
			//mLightingSystem.UpdateBuffer(Math::Vector4(mCameraSystem.GetMainCamera()->GetPosition(), 1.0f));

			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_Animation";
			CBDesc.Size = sizeof(Math::Matrix4) * 100;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mAnimationCB);

			Rendering::RenderingPipelineBakingDesc bakedesc;
			bakedesc.mRTWidth = RTWidth;
			bakedesc.mRTHeight = RTHeight;

			bakedesc.mShadingModelDesc.DirLights = static_cast<Uint32>(mLightingSystem.DirLights.size());
			bakedesc.mShadingModelDesc.SpotLights = static_cast<Uint32>(mLightingSystem.SpotLights.size());
			bakedesc.mShadingModelDesc.PointLights = static_cast<Uint32>(mLightingSystem.PointLights.size());
			bakedesc.mShadingModelDesc.CameraBufferPtr = mCameraSystem.GetCameraCB();
			bakedesc.mShadingModelDesc.LightsBufferPtr = mLightingSystem.mPSLightCB;
			bakedesc.mShadingModelDesc.AnimationBufferPtr = mAnimationCB;

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

			//TODO: Move!
			Assets::TextureSet CubeSet;
			CubeSet.mData.push_back({ 0, Managers::AssetManager::DefaultWhiteTex });
			CubeSet.mData.push_back({ 1, Managers::AssetManager::DefaultSpecularTex });
			LightSphereMaterial.mPixelShaderTextures.push_back(CubeSet);
			CreateMaterialForAllPipelines(&LightSphereMaterial);
		}
		void RenderSystem::ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight)
		{
			mActiveRenderingPipeline->ResizeRenderTargets(RTWidth, RTHeight);
		}
		Rendering::RenderingPipeline* RenderSystem::GetActivePipeline()
		{
			return mActiveRenderingPipeline;
		}
		CameraSubSystem& RenderSystem::GetCameraSubSystem()
		{
			return mCameraSystem;
		}

		LightingSubSystem& RenderSystem::GetLightingSubSystem()
		{
			return mLightingSystem;				
		}

		IBuffer* RenderSystem::GetAnimationCB()
		{
			return mAnimationCB;
		}

		void RenderSystem::Update(ECS::TimeDelta dt)
		{
			//Render Scene from each avtive camera perspective
			//for (auto Camera : mCameraSystem.ActiveCameras)
			{
				//Update Lights Positions
				auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
				for (auto entity : PointLightView)
				{
					auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
					auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
					PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
				}

				auto Camera = mCameraSystem.GetMainCamera();
				mLightingSystem.UpdateBuffer(Math::Vector4(Camera->GetPosition(), 1.0f));

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