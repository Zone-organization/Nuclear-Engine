#include <Engine\Rendering\RenderingPipeline.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine/Graphics/ImGui.h>

namespace Nuclear
{
	namespace Rendering
	{
		RenderingPipeline::RenderingPipeline()
		{
			static int id = 0;
			std::string name = "UnNamed_RenderingPipeline" + id;
			id++;

			mName = name;
			mID = Utilities::Hash(mName);

			mPostProcessingEffects[Utilities::Hash("HDR")] = Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("GAMMACORRECTION")] = Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("BLOOM")] = Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}
		RenderingPipeline::RenderingPipeline(const std::string& name)
		{
			mName = name;
			mID = Utilities::Hash(mName);

			mPostProcessingEffects[Utilities::Hash("HDR")] = Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("GAMMACORRECTION")] = Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("BLOOM")] = Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}

		void ParseForMatch(const std::unordered_map<Uint32, ShaderEffect>& first, const std::string& first_it_name, 
			const std::unordered_map<Uint32, ShaderEffect>& second, const std::string& second_it_name, std::unordered_map<Uint32, ShaderEffect>& result)
		{
			for (auto it1 : first)
			{
				//Check if id exists in result
				auto it = result.find(it1.first);
				if (it == result.end())                     //if not found
				{
					if (it1.second.GetType() == ShaderEffect::Type::PostProcessingAndRenderingEffect)
					{
						//Check if id exists in second
						auto it = second.find(it1.first);   
						if (it != second.end())               //if found
						{
							result[it1.first] = it1.second;   //Add new
						}
						else 
						{                                //ID not found (mis match)
							NUCLEAR_WARN("[RenderingPipeline] ShaderEffect: '{0}' in '{1}' has no match in '{2}'" , it1.second.GetName() , first_it_name ,  second_it_name);
						}
					}
					else
					{
						auto it = result.find(it1.first);
						if (it != result.end())
						{
							assert(false);
						}
						//Add remaining effects
						result[it1.first] = it1.second;
					}
				}
			}
		}

		Graphics::RenderTarget* RenderingPipeline::GetSceneRT()
		{
			return &SceneRT;
		}

		IPipelineState* RenderingPipeline::GetActivePipeline()
		{
			return mActivePSO.RawPtr();
		}

		IShaderResourceBinding* RenderingPipeline::GetActiveSRB()
		{
			return mActiveSRB.RawPtr();
		}
		void RenderingPipeline::Bake(const RenderingPipelineBakingDesc& desc)
		{
			ShadingModelBakingDesc SMDesc = desc.mShadingModelDesc;
			for (auto it : mPairedEffects)
			{
				SMDesc.mRequiredEffects.push_back(it.second);
			}

			GetShadingModel()->Bake(SMDesc);

			mRTWidth = desc.mRTWidth;
			mRTHeight = desc.mRTHeight;
			BakeRenderTargets();
			BakePostFXPipeline();
		}
		void RenderingPipeline::ResizeRenderTargets(Uint32 Width, Uint32 Height)
		{
			mRTWidth = Width;
			mRTHeight = Height;
			BakeRenderTargets();

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.GetSRV());
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

					BloomRT.Create(RTDesc);
				}
			}
		}
		void RenderingPipeline::SetPostProcessingEffect(const Uint32& effectId, bool value)
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
		void RenderingPipeline::SetEffect(const Uint32& effectId, bool value)
		{
			//1. Find effect
			auto it = mPairedEffects.find(effectId);
			if (it != mPairedEffects.end())
			{
				it->second.SetValue(value);
				if (it->second.GetType() == ShaderEffect::Type::RenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::PostProcessingEffect)
				{
					SetPostProcessingEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::PostProcessingAndRenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
					SetPostProcessingEffect(effectId, value);
				}
			}
			else
			{
				//No match
			}
		}

		void RenderingPipeline::InstantRender(Assets::Mesh* mesh, Assets::Material* material)
		{
			if (Engine::GetInstance()->isDebug())
			{
				if (mesh == nullptr)
				{
					NUCLEAR_ERROR("[RenderSystem DEBUG] Skipped Rendering invalid Mesh...");
					return;
				}
				if (material == nullptr)
				{
					NUCLEAR_ERROR("[RenderSystem DEBUG] Skipped Rendering Mesh with invalid Material...");
					return;
				}
			}

			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				material->GetMaterialInstance(GetShadingModel()->GetID())->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);

			}
		}

		void RenderingPipeline::SetShadingModelAndCamera(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			mShadingModel = shadingModel;
			mCamera = camera;

			//Step 1: Find Compatible Effects (build mPairedEffects map)
			ParseForMatch(mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPostProcessingEffects, "PostProcessing", mPairedEffects);
			ParseForMatch(mPostProcessingEffects, "PostProcessing", mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPairedEffects);
		}
		void RenderingPipeline::SetPostFXPipelineForRendering()
		{
			Graphics::Context::GetContext()->SetPipelineState(GetShadingModel()->GetShadersPipeline());
			std::vector<ITextureView*> RTargets;
			RTargets.push_back(GetSceneRT()->GetRTV());
			if (mBloomEnabled)
			{
				RTargets.push_back(BloomRT.GetRTV());
			}

			Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), SceneDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(GetSceneRT()->GetRTV(), (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			if (mBloomEnabled)
			{
				Graphics::Context::GetContext()->ClearRenderTarget(BloomRT.GetRTV(), (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			}
		}
		void RenderingPipeline::BakePostFXPipeline()
		{
			std::vector<LayoutElement> Layout;

			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::CompoundPipelineDesc PSOCreateInfo;

			for (auto it : mPostProcessingEffects)
			{
				PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.second.GetName()));
			}

			PSOCreateInfo.mVShaderPath = PostFX_VS_Path;
			PSOCreateInfo.mPShaderPath = PostFX_PS_Path;

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

			mPostFXPipeline.Create(PSOCreateInfo);

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

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(mRTWidth, mRTHeight);
			}

			UpdatePSO(true);
		}
		void RenderingPipeline::BakeRenderTargets()
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = mRTWidth;
			RTDesc.Height = mRTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			SceneRT.Create(RTDesc);

			RTDesc.DepthTexFormat = TEX_FORMAT_D32_FLOAT;
			SceneDepthRT.Create(RTDesc);
		}
		void RenderingPipeline::ApplyPostProcessingEffects()
		{	

			ImGui::Begin("PostFX");
			ImGui::Image(SceneRT.GetSRV(), { 256.f,256.f });

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
						mBloomBlur.SetHorizentalPSO(first_iteration ? BloomRT.GetSRV() : mBloomBlur.BlurVerticalRT.GetSRV());
					}
					else
					{
						mBloomBlur.SetVerticalPSO(mBloomBlur.BlurHorizentalRT.GetSRV());
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

				ImGui::Image(BloomRT.GetSRV(), { 256.f,256.f });
			}

			ImGui::End();
		}
		void RenderingPipeline::SetFinalPipelineState()
		{
			Graphics::Context::GetContext()->SetPipelineState(GetActivePipeline());

			GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.GetSRV());
			if (mBloomEnabled)
			{ GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mBloomBlur.BlurVerticalRT.GetSRV()); }
			Graphics::Context::GetContext()->CommitShaderResources(GetActiveSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
		void RenderingPipeline::UpdatePSO(bool ForceDirty)
		{
			mRequiredHash = 0;

			if (mPipelineDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto it : mPostProcessingEffects)
				{
					if (it.second.GetValue())
					{
						if (_hashzeroed)
						{
							mRequiredHash = 0;
							_hashzeroed = true;
						}
						auto val = mRequiredHash + it.second.GetID();

						mRequiredHash = val;
					}
				}

				auto PSO_SRB = mPostFXPipeline.GetVariant(mRequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.GetSRV());
			}
		}

		Rendering::ShadingModel* RenderingPipeline::GetShadingModel() const
		{
			return mShadingModel;
		}
		Graphics::Camera* RenderingPipeline::GetCamera() const
		{
			return mCamera;
		}

		Uint32 RenderingPipeline::GetID() const
		{
			return mID;
		}
		std::string RenderingPipeline::GetName() const
		{
			return mName;
		}
		Graphics::BakeStatus RenderingPipeline::GetStatus() const
		{
			return mStatus;
		}
	}
}