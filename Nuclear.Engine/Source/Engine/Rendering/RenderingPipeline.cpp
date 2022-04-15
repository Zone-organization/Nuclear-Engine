#include <Engine\Rendering\RenderingPipeline.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Systems\CameraSubSystem.h>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
	namespace Rendering
	{
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
						else {                                //ID not found (mis match)
							Log.Warning("[RenderingPipeline] ShaderEffect: " + it1.second.GetName() + " In " + first_it_name + " has no match in " + second_it_name + "\n");
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


				//if (it1.second.GetType() == ShaderEffect::Type::CameraAndRenderingEffect)
				//{
				//	bool matchfound = false;
				//	for (auto it2 : second)
				//	{
				//		if (!matchfound)
				//		{
				//			//Check if exists
				//			auto it = result.find(it1.second.GetID());
				//			if (it == result.end())
				//			{
				//				if (it1.second.GetID() == it2.second.GetID())
				//				{
				//					result[it1.second.GetID()] = it1.second;
				//					matchfound = true;
				//				}
				//			}
				//		}
				//	}

				//	if (!matchfound)
				//	{
				//		//Log.Warning("[RenderingPipeline] Shading Model: " + std::to_string(shadingModel->GetID()) +
				//		//	" ShaderEffect: " + sm_it.GetName() + " Has no match\n");
				//		Log.Warning("[RenderingPipeline] ShaderEffect: " + it1.second.GetName() + " In " + first_it_name + " has no match in " + second_it_name + "\n");
				//	}
				//}
				//else 
				//{
				//	auto it = result.find(it1.second.GetID());
				//	if (it != result.end())
				//	{
				//		assert(false);
				//	}
				//	//Add remaining effects
				//	result[it1.second.GetID()] = it1.second;
				//}
			}
		}

		void RenderingPipeline::Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			mShadingModel = shadingModel;
			mCamera = camera;


			//Step 1: Find Compatible Effects (build mPairedEffects map)
			ParseForMatch(mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPostProcessingEffects, "PostProcessing", mPairedEffects);
			ParseForMatch(mPostProcessingEffects,"PostProcessing", mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPairedEffects);

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

		void RenderingPipeline::StartRendering()
		{
			std::vector<ITextureView*> RTargets;
			RTargets.push_back(GetSceneRT()->mColorRTV);
			if (mBloomEnabled)
			{
				RTargets.push_back(BloomRT.mColorRTV);
			}

			Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), GetSceneRT()->mDepthDSV.RawPtr(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(GetSceneRT()->mColorRTV.RawPtr(), (float*)&mCamera->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			if (mBloomEnabled)
			{
				Graphics::Context::GetContext()->ClearRenderTarget(BloomRT.mColorRTV, (float*)&mCamera->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			}
			Graphics::Context::GetContext()->ClearDepthStencil(GetSceneRT()->mDepthDSV.RawPtr(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		void RenderingPipeline::ApplyPostProcessingEffects()
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

		void RenderingPipeline::SetPipelineState()
		{
			Graphics::Context::GetContext()->SetPipelineState(GetActivePipeline());

			GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);

			if (mBloomEnabled)
			{
				GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mBloomBlur.BlurVerticalRT.mShaderRTV);
			}

			Graphics::Context::GetContext()->CommitShaderResources(GetActiveSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		}

		void RenderingPipeline::BakeRenderTarget()
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			SceneRT.Create(RTDesc);
		}

		void RenderingPipeline::UpdatePSO(bool ForceDirty)
		{
			RequiredHash = 0;

			if (mPipelineDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto it : mPostProcessingEffects)
				{
					if (it.second.GetValue())
					{
						if (_hashzeroed)
						{
							RequiredHash = 0;
							_hashzeroed = true;
						}
						auto val = RequiredHash + it.second.GetID();

						RequiredHash = val;
					}
				}

				auto PSO_SRB = mPipeline.GetVariant(RequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			}
		}


		void RenderingPipeline::ResizeRenderTarget(Uint32 Width, Uint32 Height)
		{
			RTWidth = Width;
			RTHeight = Height;
			BakeRenderTarget();

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			auto it = mPostProcessingEffects.find(Utilities::Hash("BLOOM"));
			if (it != mPostProcessingEffects.end())
			{
				if (it->second.GetValue())
				{
					mBloomBlur.ResizeRenderTargets(RTWidth, RTHeight);
					Graphics::RenderTargetDesc RTDesc;
					RTDesc.Width = RTWidth;
					RTDesc.Height = RTHeight;
					RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
					RTDesc.mCreateDepth = false;

					BloomRT.Create(RTDesc);
				}
			}
		}


		void RenderingPipeline::BakePipeline()
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
				RTDesc.Width = RTWidth;
				RTDesc.Height = RTHeight;
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
				RTDesc.mCreateDepth = false;

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(RTWidth, RTHeight);
			}

			UpdatePSO(true);
		}

		Rendering::ShadingModel* RenderingPipeline::GetShadingModel()
		{
			return mShadingModel;
		}
		Graphics::Camera* RenderingPipeline::GetCamera()
		{
			return mCamera;
		}
		void RenderingPipeline::Bake(ShadingModelBakingDesc& desc, Uint32 Width, Uint32 Height)
		{
			for (auto it : mPairedEffects)
			{
				desc.mRequiredEffects.push_back(it.second);
			}

			mShadingModel->Bake(desc);

			RTWidth = Width;
			RTHeight = Height;
			BakeRenderTarget();
			BakePipeline();
		}
		Uint32 RenderingPipeline::GetID() const
		{
			return mID;
		}
		std::string RenderingPipeline::GetName() const
		{
			return mName;
		}
	}
}