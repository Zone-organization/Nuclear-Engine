#include "Engine/Rendering/ForwardRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Components\AnimatorComponent.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Systems\RenderSystem.h>

namespace Nuclear
{
	namespace Rendering
	{
		ForwardRenderingPipeline::ForwardRenderingPipeline(const std::string& name)
			: RenderingPipeline(name)
		{
			mPostProcessingEffects[Utilities::Hash("HDR")] = Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("GAMMACORRECTION")] = Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("BLOOM")] = Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}
		void ForwardRenderingPipeline::Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			SetShadingModelAndCamera(shadingModel, camera);
		}
		void ForwardRenderingPipeline::Bake(const RenderingPipelineBakingDesc& desc)
		{
			ShadingModelBakingDesc SMDesc = desc.mShadingModelDesc;
			for (auto it : mPairedEffects)
			{
				SMDesc.mRequiredEffects.push_back(it.second);
			}

			GetShadingModel()->Bake(SMDesc);

			mRTWidth = desc.mRTWidth;
			mRTHeight = desc.mRTHeight;
			BakeRenderTarget();
			BakePipeline();
		}


		void ForwardRenderingPipeline::BakeRenderTarget()
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = mRTWidth;
			RTDesc.Height = mRTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			SceneRT.Create(RTDesc);
		}
		void ForwardRenderingPipeline::BakePipeline()
		{
			std::vector<LayoutElement> Layout;

			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::CompoundPipelineDesc PSOCreateInfo;

			for (auto it : mPostProcessingEffects)
			{
				PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.second.GetName()));
			}

			PSOCreateInfo.mVShaderPath = VS_Path;
			PSOCreateInfo.mPShaderPath = PS_Path;

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

			mPipeline.Create(PSOCreateInfo);

			bool bloomincluded = false;
			for (auto it : mPostProcessingEffects)
			{
				if (it.second.GetName() == "BLOOM")
				{
					bloomincluded = true;
				}
			}

			//Create Blur pipeline
			if (bloomincluded)
			{
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.Width = mRTWidth;
				RTDesc.Height = mRTHeight;
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
				RTDesc.mCreateDepth = false;

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(mRTWidth, mRTHeight);
			}

			UpdatePSO(true);
		}

		void ForwardRenderingPipeline::ResizeRenderTarget(Uint32 Width, Uint32 Height)
		{
			mRTWidth = Width;
			mRTHeight = Height;
			BakeRenderTarget();

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			auto it = mPostProcessingEffects.find(Utilities::Hash("BLOOM"));
			if (it != mPostProcessingEffects.end())
			{
				if (it->second.GetValue())
				{
					mBloomBlur.ResizeRenderTargets(mRTWidth, mRTHeight);
					Graphics::RenderTargetDesc RTDesc;
					RTDesc.Width = mRTWidth;
					RTDesc.Height = mRTHeight;
					RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
					RTDesc.mCreateDepth = false;

					BloomRT.Create(RTDesc);
				}
			}
		}

		void ForwardRenderingPipeline::SetPostProcessingEffect(const Uint32& effectId, bool value)
		{
			auto it = mPostProcessingEffects.find(effectId);
			if (it != mPostProcessingEffects.end())
			{
				it->second.SetValue(value);
				if (it->second.GetName() == "BLOOM")
				{
					mBloomEnabled = value;
				}
			}
			else
			{
				assert(false);
			}
			mPipelineDirty = true;
		}

		void ForwardRenderingPipeline::StartRendering(Systems::RenderSystem* renderer)
		{
			std::vector<ITextureView*> RTargets;
			RTargets.push_back(GetSceneRT()->mColorRTV);
			if (mBloomEnabled)
			{
				RTargets.push_back(BloomRT.mColorRTV);
			}

			Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), GetSceneRT()->mDepthDSV.RawPtr(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(GetSceneRT()->mColorRTV.RawPtr(), (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			if (mBloomEnabled)
			{
				Graphics::Context::GetContext()->ClearRenderTarget(BloomRT.mColorRTV, (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			}
			Graphics::Context::GetContext()->ClearDepthStencil(GetSceneRT()->mDepthDSV.RawPtr(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


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
					renderer->GetCameraSubSystem().UpdateBuffer();

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
						data = memcpy(data, &empty,  sizeof(Math::Matrix4));
						Graphics::Context::GetContext()->UnmapBuffer(renderer->GetAnimationCB(), MAP_WRITE);
					}

					InstantRender(MeshObject.mMesh, MeshObject.mMaterial);
				}
			}

			if (renderer->VisualizePointLightsPositions)
			{
				for (unsigned int i = 0; i < renderer->GetLightingSubSystem().PointLights.size(); i++)
				{
					Math::Matrix4 model(1.0f);
					model = Math::translate(model, Math::Vector3(renderer->GetLightingSubSystem().PointLights[i]->GetInternalData().Position));
					model = Math::scale(model, Math::Vector3(0.75f));
					GetCamera()->SetModelMatrix(model);
					renderer->GetCameraSubSystem().UpdateBuffer();
					InstantRender(Assets::DefaultMeshes::GetSphereAsset(), &renderer->LightSphereMaterial);
				}
			}

			if (GetCamera()->RenderSkybox == true && GetCamera()->mSkybox != nullptr)
			{
				GetCamera()->mSkybox->Render();
			}

			ApplyPostProcessingEffects();
		}

		void ForwardRenderingPipeline::SetPipelineState()
		{
			Graphics::Context::GetContext()->SetPipelineState(GetActivePipeline());

			GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			if (mBloomEnabled)
			{
				GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mBloomBlur.BlurVerticalRT.mShaderRTV);
			}
			Graphics::Context::GetContext()->CommitShaderResources(GetActiveSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		void ForwardRenderingPipeline::ApplyPostProcessingEffects()
		{
			//Apply blur
			if (mBloomEnabled)
			{
				bool horizontal = true, first_iteration = true, horicleared = false, verticleared = false;
				int amount = 10;


				for (unsigned int i = 0; i < amount; i++)
				{
					Math::Vector4i data;

					if (horizontal)
					{
						mBloomBlur.SetHorizentalPSO(first_iteration ? BloomRT.mShaderRTV : mBloomBlur.BlurVerticalRT.mShaderRTV);
					}
					else
					{
						mBloomBlur.SetVerticalPSO(mBloomBlur.BlurHorizentalRT.mShaderRTV);
					}

					//X: Texture Width
					//Y: Texutre Height
					//Z: Padding
					//W: Padding
					{
						data.x = BloomRT.GetWidth();
						data.y = BloomRT.GetHeight();

						Diligent::MapHelper<Math::Vector4i> CBConstants(Graphics::Context::GetContext(), mBloomBlur.mBlurCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = data;
					}


					if (horizontal)
					{
						Graphics::Context::GetContext()->CommitShaderResources(mBloomBlur.mHorzBlurSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					}
					else
					{
						Graphics::Context::GetContext()->CommitShaderResources(mBloomBlur.mVertBlurSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					}
					Assets::DefaultMeshes::RenderScreenQuad();

					horizontal = !horizontal;
					if (first_iteration)
						first_iteration = false;
				}
			}
		}
	}
}