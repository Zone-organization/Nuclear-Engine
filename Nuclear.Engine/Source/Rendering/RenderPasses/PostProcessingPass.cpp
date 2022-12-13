#include "Rendering\RenderPasses\PostProcessingPass.h"
#include <Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Assets\DefaultMeshes.h>
#include <Rendering/FrameRenderData.h>
#include <Platform\FileSystem.h>

namespace Nuclear
{
	namespace Rendering
	{
		PostProcessingPass::PostProcessingPass()
		{
			_HashedBloomID = Utilities::Hash("BLOOM");
		}

		void PostProcessingPass::Update(FrameRenderData* framedata)
		{
			mPipelineCntrllr.Update();
			auto pipeline = mPipelineCntrllr.GetActiveVariant()->GetMainPipeline();
			auto pipelineSRB = mPipelineCntrllr.GetActiveVariant()->GetMainPipelineSRB();

			//Render Skybox

			if (GetBackground().GetSkybox() != nullptr)
			{
				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, framedata->mFinalRT.GetRTVDblPtr(), framedata->mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				GetBackground().GetSkybox()->Render();
			}

			if (mBloomEnabled)
			{		
				//1 - Extract bloom from scene rt
				Graphics::Context::GetInstance().GetContext()->SetPipelineState(pBloomExtractPSO);
				pBloomExtractSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(framedata->mFinalRT.GetSRV());
				Graphics::Context::GetInstance().GetContext()->CommitShaderResources(pBloomExtractSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, BloomRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(BloomRT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Assets::DefaultMeshes::RenderScreenQuad();


				//2 - apply bloom blur
				bool horizontal = true, first_iteration = true, horicleared = false, verticleared = false;
				unsigned int amount = 10;

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
						data.x = BloomRT.GetDimensions().x;
						data.y = BloomRT.GetDimensions().y;

						Diligent::MapHelper<Math::Vector4i> CBConstants(Graphics::Context::GetInstance().GetContext(), mBloomBlur.mBlurCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = data;
					}


					if (horizontal)
					{
						Graphics::Context::GetInstance().GetContext()->CommitShaderResources(mBloomBlur.mHorzBlurSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					}
					else
					{
						Graphics::Context::GetInstance().GetContext()->CommitShaderResources(mBloomBlur.mVertBlurSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
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
			Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, PostFXRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(PostFXRT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetInstance().GetContext()->SetPipelineState(pipeline);

			pipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(framedata->mFinalRT.GetSRV());
			if (mBloomEnabled)
			{
				pipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mBloomBlur.BlurVerticalRT.GetSRV());
			}
			Graphics::Context::GetInstance().GetContext()->CommitShaderResources(pipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Assets::DefaultMeshes::RenderScreenQuad();


			//4- copy results to FinalRT
			CopyTextureAttribs attrib;
			attrib.pSrcTexture = PostFXRT.GetSRV()->GetTexture();
			attrib.pDstTexture = framedata->mFinalRT.GetSRV()->GetTexture();
			Graphics::Context::GetInstance().GetContext()->CopyTexture(attrib);
		
		}

		void PostProcessingPass::SetPostProcessingEffect(Uint32 effectId, bool value)
		{
			mPipelineCntrllr.SetSwitch(effectId, value);

			//check for bloom
			if (effectId == _HashedBloomID)
			{
				mBloomEnabled = value;
			}

		}

		Graphics::ShaderPipelineSwitchController& PostProcessingPass::GetPipelineController()
		{
			return mPipelineCntrllr;
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

			PostFXRT.Resize(Math::Vector2ui(Width, Height));


		//	mPipelineCntrllr.GetActiveVariant()->GetMainPipelineSRB().Release();
		//	mPipelineCntrllr.GetActiveVariant()->GetMainPipeline()->CreateShaderResourceBinding(mPipelineCntrllr.GetActiveVariant()->GetMainPipelineSRB(), true);
		
		
		//mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.GetSRV());

			if (mPipelineCntrllr.GetSwitch(_HashedBloomID).GetValue())
			{
				mBloomBlur.ResizeRenderTargets(mRTWidth, mRTHeight);
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.mDimensions = Math::Vector2ui(mRTWidth, mRTHeight);
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

				BloomRT.Create(RTDesc);
			}
		}

		void PostProcessingPass::BakePostFXPipeline()
		{
			std::vector<LayoutElement> Layout;

			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			{

				Graphics::ShaderPipelineDesc PSOCreateInfo;
				PSOCreateInfo.Switches.push_back(Graphics::ShaderPipelineSwitch("HDR", true));
				PSOCreateInfo.Switches.push_back(Graphics::ShaderPipelineSwitch("GAMMACORRECTION", true));
				PSOCreateInfo.Switches.push_back(Graphics::ShaderPipelineSwitch("BLOOM", false));
				PSOCreateInfo.mForwardPSOCreateInfo.mVertexShader.mType = SHADER_TYPE_VERTEX;
				PSOCreateInfo.mForwardPSOCreateInfo.mVertexShader.mPath = mDesc.PostFX_VS_Path;
				PSOCreateInfo.mForwardPSOCreateInfo.mPixelShader.mPath = mDesc.PostFX_PS_Path;
				PSOCreateInfo.mForwardPSOCreateInfo.mPixelShader.mType = SHADER_TYPE_PIXEL;

				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
				PSOCreateInfo.mForwardPSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

				mPostFXPipeline.Create(PSOCreateInfo);
				mPostFXPipeline.Bake(nullptr);
			}

			//bloom
			{
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.mDimensions = Math::Vector2ui(mRTWidth, mRTHeight);

				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(mRTWidth, mRTHeight);


				//Create bloom extract PSO

				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "BloomExtract PSO";
				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = TEX_FORMAT_RGBA16_FLOAT;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				//Create Vertex Shader
				{

					ShaderCreateInfo CreationAttribs;

					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "BloomExtractVS";

					auto source = Platform::FileSystem::LoadShader(mDesc.PostFX_VS_Path, std::set<std::string>(), std::set<std::string>(), true);
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsEngine::GetInstance().GetDefaultShaderSourceFactory();

					Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, VSShader.RawDblPtr());
				}

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;

					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "BloomExtractPS";

					auto source = Platform::FileSystem::LoadShader("@NuclearAssets@/Shaders/BloomExtract.ps.hlsl", std::set<std::string>(), std::set<std::string>(), true);
					CreationAttribs.Source = source.c_str();
					Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, PSShader.RawDblPtr());
				}

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

				auto Vars = Graphics::GraphicsEngine::GetInstance().ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetInstance().ProcessAndCreatePipeline(&pBloomExtractPSO, PSOCreateInfo, Vars, true);
				pBloomExtractPSO->CreateShaderResourceBinding(pBloomExtractSRB.RawDblPtr());
			}

			mPipelineCntrllr.SetPipeline(&mPostFXPipeline);
			mPipelineCntrllr.Update();
		}


		Rendering::Background& PostProcessingPass::GetBackground()
		{
			return mBackground;
		}

	}
}