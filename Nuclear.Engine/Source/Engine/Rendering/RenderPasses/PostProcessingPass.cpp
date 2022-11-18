#include "Engine\Rendering\RenderPasses\PostProcessingPass.h"
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine/Rendering/FrameRenderData.h>
#include <Core\FileSystem.h>

namespace Nuclear
{
	namespace Rendering
	{
		PostProcessingPass::PostProcessingPass()
		{
			mPostProcessingEffects[Utilities::Hash("HDR")] = Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::PostProcessingEffect, true);
			mPostProcessingEffects[Utilities::Hash("GAMMACORRECTION")] = Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::PostProcessingEffect, true);
			mPostProcessingEffects[Utilities::Hash("BLOOM")] = Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
		}

		void PostProcessingPass::Update(FrameRenderData* framedata)
		{
			//mBloomEnabled = true;
			UpdatePSO();
			//1 - Extract bloom from scene rt
			if (mBloomEnabled)
			{
				Graphics::Context::GetContext()->SetPipelineState(pBloomExtractPSO);
				pBloomExtractSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(framedata->mFinalRT.GetSRV());
				Graphics::Context::GetContext()->CommitShaderResources(pBloomExtractSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::Context::GetContext()->SetRenderTargets(1, BloomRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->ClearRenderTarget(BloomRT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Assets::DefaultMeshes::RenderScreenQuad();
			}

			//2 - apply bloom blur
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

				//TODO: Add to DEBUGSYSTEM RTs
				//ImGui::Image(BloomRT.GetSRV(), { 256.f,256.f });
			}

			//3 - apply the remaning effects
			Graphics::Context::GetContext()->SetPipelineState(GetActivePipeline());

			GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(framedata->mFinalRT.GetSRV());
			if (mBloomEnabled)
			{
				GetActiveSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mBloomBlur.BlurVerticalRT.GetSRV());
			}
			Graphics::Context::GetContext()->CommitShaderResources(GetActiveSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetContext()->SetRenderTargets(1, PostFXRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(PostFXRT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Assets::DefaultMeshes::RenderScreenQuad();


			//4- copy results to FinalRT
			CopyTextureAttribs attrib;
			attrib.pSrcTexture = PostFXRT.GetSRV()->GetTexture();
			attrib.pDstTexture = framedata->mFinalRT.GetSRV()->GetTexture();
			Graphics::Context::GetContext()->CopyTexture(attrib);
		}

		void PostProcessingPass::SetPostProcessingEffect(const Uint32& effectId, bool value)
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
		}

		void PostProcessingPass::UpdatePSO(bool ForceDirty)
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

				mActivePSO = PSO_SRB.GetRenderingPipeline();
				mActiveSRB = PSO_SRB.GetRenderingSRB();
			}
		}
	

		IPipelineState* PostProcessingPass::GetActivePipeline()
		{
			return mActivePSO.RawPtr();
		}

		IShaderResourceBinding* PostProcessingPass::GetActiveSRB()
		{
			return mActiveSRB.RawPtr();
		}
		void PostProcessingPass::Bake(const PostProcessingBakingDesc& desc)
		{
			mRTWidth = desc.mRTWidth;
			mRTHeight = desc.mRTHeight;
			PostFXRT.Create(desc.mFinalRTDesc);
			BakePostFXPipeline();
		}

		void PostProcessingPass::ResizeRTs(Uint32 Width, Uint32 Height)
		{
			mRTWidth = Width;
			mRTHeight = Height;
			PostFXRT.Resize(Width, Height);

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			//mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.GetSRV());
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

		void PostProcessingPass::BakePostFXPipeline()
		{

			{
				std::vector<LayoutElement> Layout;

				Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
				Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
				Graphics::ShaderPipelineDesc PSOCreateInfo;

				for (auto it : mPostProcessingEffects)
				{
					PSOCreateInfo.Switches.push_back(Graphics::ShaderPipelineSwitch(it.second.GetName()));
				}

				PSOCreateInfo.mVertexShader.mPath = mDesc.PostFX_VS_Path;
				PSOCreateInfo.mPixelShader.mPath = mDesc.PostFX_PS_Path;

				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

				mPostFXPipeline.Create(PSOCreateInfo);
			}

			//bloom
			{
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.Width = mRTWidth;
				RTDesc.Height = mRTHeight;
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(mRTWidth, mRTHeight);


				//Create bloom extract PSO

				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "BloomExtract PSO";
				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(false);

				//Create Vertex Shader
				{

					ShaderCreateInfo CreationAttribs;

					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "BloomExtractVS";

					auto source = Core::FileSystem::LoadShader(mDesc.PostFX_VS_Path, std::vector<std::string>(), std::vector<std::string>(), true);
					CreationAttribs.Source = source.c_str();
					RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
					Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
					CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;
					Graphics::Context::GetDevice()->CreateShader(CreationAttribs, VSShader.RawDblPtr());
				}

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;

					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "BloomExtractPS";

					auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/BloomExtract.ps.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
					CreationAttribs.Source = source.c_str();
					Graphics::Context::GetDevice()->CreateShader(CreationAttribs, PSShader.RawDblPtr());
				}

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&pBloomExtractPSO, PSOCreateInfo, Vars, true);
				pBloomExtractPSO->CreateShaderResourceBinding(pBloomExtractSRB.RawDblPtr());
			}

			UpdatePSO(true);
		}
	}
}