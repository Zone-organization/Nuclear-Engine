#include <Rendering/RenderPasses/AmbientOcclusionPass.h>
#include <Assets/AssetManager.h>
#include <Assets/Shader.h>
#include <Graphics/Context.h>
#include <random>

namespace Nuclear
{
	namespace Rendering
	{
		using namespace Diligent;
#define SAMPLES_NUM 64

		void AmbientOcclusionPass::Bake(Uint32 RTWidth, Uint32 RTHeight)
		{
			Assets::ShaderImportingDesc desc;
			desc.mCommonOptions.mLoadOnly = true;

			pSSAO_ExtractShader = Assets::AssetManager::GetInstance().Import<Assets::Shader>("@NuclearAssets@/Shaders/SSAO_Extract.NuclearShader");
		
			//create cb
			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_SSAO_Samples";
			CBDesc.Size = (sizeof(Math::Vector3) * SAMPLES_NUM) + sizeof(Math::Matrix4);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pSSAO_ExtractCB);



			// generate sample kernel
			 // ----------------------
			std::uniform_real_distribution<Float32> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
			std::default_random_engine generator;
			std::vector<glm::vec3> ssaoKernel;
			for (unsigned int i = 0; i < 64; ++i)
			{
				glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
				sample = glm::normalize(sample);
				sample *= randomFloats(generator);
				float scale = float(i) / 64.0f;

				// scale samples s.t. they're more aligned to center of kernel
				scale = std::lerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				ssaoKernel.push_back(sample);
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
			Graphics::ShaderPipelineBakingDesc bakedesc;
			bakedesc.pVariantsFactory = &Graphics::GraphicsEngine::GetInstance().GetDefaultShaderPipelineVariantFactory();
			bakedesc.mStaticVariablesBindings.push_back({ SHADER_TYPE_PIXEL ,"NEStatic_SSAO_Samples" ,pSSAO_ExtractCB });
			bakedesc.mStaticVariablesBindings.push_back({ SHADER_TYPE_PIXEL ,"NE_SSAO_Noise" ,pNoiseTexView });

			pSSAO_ExtractShader->GetShaderPipeline().Bake(bakedesc);


			//create rt
			Graphics::RenderTargetDesc rtdesc;
			rtdesc.mName = "SSAO_RT";
			rtdesc.mType = "AmbientOcclusion";
			rtdesc.ColorTexFormat = TEX_FORMAT_R32_FLOAT;
			rtdesc.mDimensions = Math::Vector2ui(RTWidth, RTHeight);
			mSSAO_RT.Create(rtdesc);

		}
		void AmbientOcclusionPass::Update(FrameRenderData* framedata)
		{
		}
		void AmbientOcclusionPass::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
			mSSAO_RT.Resize(Math::Vector2ui(RTWidth, RTHeight));
		}
	}
}