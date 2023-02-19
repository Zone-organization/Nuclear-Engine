#include <Rendering/RenderPasses/AmbientOcclusionPass.h>
#include <Assets/AssetManager.h>
#include <Assets/Shader.h>
#include <Graphics/Context.h>
#include <random>
#include <Rendering\FrameRenderData.h>
#include <Assets\DefaultMeshes.h>
#include <Utilities/Logger.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>

namespace Nuclear
{
	namespace Rendering
	{
		using namespace Diligent;
#define SAMPLES_NUM 64


		struct SSAO_CB
		{
			Math::Matrix4 CameraProj;
			Math::Vector4 Samples[SAMPLES_NUM];
		};
		static SSAO_CB cbdata;
		void AmbientOcclusionPass::Bake(Uint32 RTWidth, Uint32 RTHeight)
		{
			Assets::ShaderImportingDesc desc;
			desc.mCommonOptions.mLoadOnly = true;

			pSSAO_ExtractShader = Assets::AssetManager::GetInstance().Import<Assets::Shader>("@NuclearAssets@/Shaders/SSAO_Extract.NuclearShader");
			pSSAO_BlurShader = Assets::AssetManager::GetInstance().Import<Assets::Shader>("@NuclearAssets@/Shaders/SSAO_Blur.NuclearShader");

			auto test = sizeof(SSAO_CB);
			auto test2 = (sizeof(Math::Vector3) * SAMPLES_NUM) + sizeof(Math::Matrix4);

			//create cb
			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_SSAO_Samples";
			CBDesc.Size = sizeof(SSAO_CB);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pSSAO_ExtractCB);

			// generate sample kernel
			 // ----------------------
			std::uniform_real_distribution<Float32> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
			std::default_random_engine generator;
			for (unsigned int i = 0; i < SAMPLES_NUM; ++i)
			{
				glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
				sample = glm::normalize(sample);
				sample *= randomFloats(generator);
				float scale = float(i) / 64.0f;

				// scale samples s.t. they're more aligned to center of kernel
				scale = std::lerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				cbdata.Samples[i] = Math::Vector4(sample,0.0f);
			}
			// generate noise texture
			// ----------------------
			std::vector<glm::vec3> ssaoNoise;
			for (unsigned int i = 0; i < 16; i++)
			{
				glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
				ssaoNoise.push_back(noise);
			}

			Diligent::RefCntAutoPtr<Diligent::ITexture> pNoiseTex;

			TextureSubResData data;
			data.pData = &ssaoNoise[0];
			data.Stride = sizeof(Float32);

			Diligent::TextureDesc TexDesc;
			TexDesc.Name = "SSAO_NoiseTex";
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = 4;
			TexDesc.Height = 4;
			TexDesc.Format = TEX_FORMAT_R32_FLOAT;
			TexDesc.Usage = USAGE_DEFAULT;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE;

			Diligent::TextureData TexData;
			TexData.pSubResources = &data;
			TexData.NumSubresources = 1;

			Graphics::Context::GetInstance().GetDevice()->CreateTexture(TexDesc, &TexData, pNoiseTex.RawDblPtr());

			pNoiseTexView = pNoiseTex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

			//bake pipeline
			{
				Graphics::ShaderPipelineBakingDesc bakedesc;
				bakedesc.pVariantsFactory = &Graphics::GraphicsEngine::GetInstance().GetDefaultShaderPipelineVariantFactory();
				bakedesc.mStaticVariablesBindings.push_back({ SHADER_TYPE_PIXEL ,"NEStatic_SSAO_Samples" ,pSSAO_ExtractCB });
				bakedesc.mStaticVariablesBindings.push_back({ SHADER_TYPE_PIXEL ,"NE_SSAO_Noise" ,pNoiseTexView });

				pSSAO_ExtractShader->GetShaderPipeline().Bake(bakedesc);
			}

			{

				Graphics::ShaderPipelineBakingDesc bakedesc;
				bakedesc.pVariantsFactory = &Graphics::GraphicsEngine::GetInstance().GetDefaultShaderPipelineVariantFactory();
				pSSAO_BlurShader->GetShaderPipeline().Bake(bakedesc);
			}

			//create rt
			Graphics::RenderTargetDesc rtdesc;
			rtdesc.mName = "SSAO_Extract_RT";
			rtdesc.mType = "AmbientOcclusion";
			rtdesc.ColorTexFormat = TEX_FORMAT_R32_FLOAT;
			rtdesc.mDimensions = Math::Vector2ui(RTWidth, RTHeight);
			mSSAO_Extract_RT.Create(rtdesc);

			rtdesc.mName = "SSAO_Blur_RT";
			mSSAO_Blur_RT.Create(rtdesc);
		}


		void AmbientOcclusionPass::Update(FrameRenderData* framedata)
		{
			if (framedata->mUsedDefferedPipelines.size() > 0)
			{
				auto SSAO_ExtractPipeline = pSSAO_ExtractShader->GetShaderPipeline().GetVariant(0)->GetRenderingPipeline();
				auto SSAO_ExtractPipelineSRB = pSSAO_ExtractShader->GetShaderPipeline().GetVariant(0)->GetRenderingSRB();
				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, mSSAO_Extract_RT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(mSSAO_Extract_RT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::Context::GetInstance().GetContext()->SetPipelineState(SSAO_ExtractPipeline);

				cbdata.CameraProj = framedata->pCamera->GetProjectionMatrix();
				{
					Diligent::MapHelper<SSAO_CB> CBConstants(Graphics::Context::GetInstance().GetContext(), pSSAO_ExtractCB, MAP_WRITE, MAP_FLAG_DISCARD);
					*CBConstants = cbdata;
				}
				for (auto deffered_Variant : framedata->mUsedDefferedPipelines)
				{
					SSAO_ExtractPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(deffered_Variant->pParent->GetGBuffer()->mRenderTargets.at(0).GetSRV());
					SSAO_ExtractPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(deffered_Variant->pParent->GetGBuffer()->mRenderTargets.at(1).GetSRV());

					Graphics::Context::GetInstance().GetContext()->CommitShaderResources(SSAO_ExtractPipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					Assets::DefaultMeshes::RenderScreenQuad();
				}

				//Blur
				auto SSAO_BlurPipeline = pSSAO_BlurShader->GetShaderPipeline().GetVariant(0)->GetRenderingPipeline();
				auto SSAO_BlurPipelineSRB = pSSAO_BlurShader->GetShaderPipeline().GetVariant(0)->GetRenderingSRB();
				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, mSSAO_Blur_RT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(mSSAO_Blur_RT.GetRTV(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::Context::GetInstance().GetContext()->SetPipelineState(SSAO_BlurPipeline);
				SSAO_BlurPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(mSSAO_Extract_RT.GetSRV());
				Graphics::Context::GetInstance().GetContext()->CommitShaderResources(SSAO_BlurPipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Assets::DefaultMeshes::RenderScreenQuad();
			}
		}
		void AmbientOcclusionPass::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
			mSSAO_Extract_RT.Resize(Math::Vector2ui(RTWidth, RTHeight));
			mSSAO_Blur_RT.Resize(Math::Vector2ui(RTWidth, RTHeight));
		}
		Diligent::ITextureView* AmbientOcclusionPass::GetSSAO_SRV()
		{
			return mSSAO_Blur_RT.GetSRV();
		}
	}
}