#include "Engine/Rendering/ForwardRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Components\AnimatorComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Systems\LightingSystem.h>

namespace Nuclear
{
	namespace Rendering
	{

		void ForwardRenderingPipeline::Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			SetShadingModelAndCamera(shadingModel, camera);
		}

		void ForwardRenderingPipeline::StartRendering(Systems::RenderSystem* renderer)
		{
			SetPostFXPipelineForRendering();
			Graphics::Context::GetContext()->ClearDepthStencil(SceneDepthRT.GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			//Render Meshes
			Graphics::Context::GetContext()->SetPipelineState(GetShadingModel()->GetShadersPipeline());



			auto view = renderer->mScene->GetRegistry().view<Components::MeshComponent>();

			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.mRender)
				{
					auto& EntityInfo = renderer->mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
					EntityInfo.mTransform.Update();
					GetCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
					renderer->GetCameraSystem()->UpdateBuffer();

					auto AnimatorComponent = renderer->mScene->GetRegistry().try_get<Components::AnimatorComponent>(entity);

					if (AnimatorComponent != nullptr)
					{
						std::vector<Math::Matrix4> ok;
						ok.reserve(100);

						auto transforms = AnimatorComponent->mAnimator->GetFinalBoneMatrices();
						for (int i = 0; i < transforms.size(); ++i)
						{
							ok.push_back(transforms[i]);
						}

						PVoid data;
						Graphics::Context::GetContext()->MapBuffer(renderer->GetAnimationCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, ok.data(), ok.size() * sizeof(Math::Matrix4));
						Graphics::Context::GetContext()->UnmapBuffer(renderer->GetAnimationCB(), MAP_WRITE);
					}
					else {
						Math::Matrix4 empty(0.0f);
						PVoid data;
						Graphics::Context::GetContext()->MapBuffer(renderer->GetAnimationCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, &empty, sizeof(Math::Matrix4));
						Graphics::Context::GetContext()->UnmapBuffer(renderer->GetAnimationCB(), MAP_WRITE);
					}
					//IBL
					for (int i = 0; i < GetShadingModel()->mIBLTexturesInfo.size(); i++)
					{
						GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, GetShadingModel()->mIBLTexturesInfo.at(i).mSlot)->Set(GetShadingModel()->mIBLTexturesInfo.at(i).mTex.GetImage()->mTextureView);
					}

					//Shadows
					////////////////////////     TODO    //////////////////////////////////////
					if (GetShadingModel()->mDirLight_ShadowmapInfo.mType != Assets::ShaderTextureType::Unknown)
					{

						auto DirLightView = renderer->mScene->GetRegistry().view<Components::DirLightComponent>();
						for (auto entity : DirLightView)
						{
							auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);

							GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, GetShadingModel()->mDirLight_ShadowmapInfo.mSlot)->Set(DirLight.GetShadowMap()->GetSRV());

						}
					}
					if (GetShadingModel()->mSpotLight_ShadowmapInfo.mType != Assets::ShaderTextureType::Unknown)
					{

						auto SpotLightView = renderer->mScene->GetRegistry().view<Components::SpotLightComponent>();
						for (auto entity : SpotLightView)
						{
							auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);

							GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, GetShadingModel()->mSpotLight_ShadowmapInfo.mSlot)->Set(SpotLight.GetShadowMap()->GetSRV());

						}
					}

					InstantRender(MeshObject.mMesh, MeshObject.mMaterial);
				}
			}

			//if (renderer->VisualizePointLightsPositions)
			//{
			//	for (unsigned int i = 0; i < renderer->GetLightingSubSystem().PointLights.size(); i++)
			//	{
			//		Math::Matrix4 model(1.0f);
			//		model = Math::translate(model, Math::Vector3(renderer->GetLightingSubSystem().PointLights[i]->GetInternalData().Position));
			//		model = Math::scale(model, Math::Vector3(0.75f));
			//		GetCamera()->SetModelMatrix(model);
			//		renderer->GetCameraSubSystem().UpdateBuffer();
			//		InstantRender(Assets::DefaultMeshes::GetSphereAsset(), &renderer->LightSphereMaterial);
			//	}
			//}

			if (renderer->GetBackground().GetSkybox() != nullptr)
			{
				renderer->GetBackground().GetSkybox()->Render();
			}

			ApplyPostProcessingEffects();
		}
	}
}