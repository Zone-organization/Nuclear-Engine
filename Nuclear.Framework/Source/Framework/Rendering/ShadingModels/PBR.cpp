#include <Framework\Rendering\ShadingModels\PBR.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>

namespace Nuclear
{
	namespace Rendering
	{
		PBR::PBR()
		{
			static int id = 0;
			mID = Utilities::Hash("NE_PBR_NO_IBL" + id);
			id++;

			mName = "NE_PBR_NO_IBL";

			mRenderingEffects[Utilities::Hash("BLOOM")] = ShaderEffect("BLOOM", ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}
		bool PBR::Bake(const ShadingModelBakingDesc& desc)
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "PBR_PSO";

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
			//Check for bloom
			for (auto it : desc.mRequiredEffects)
			{
				if (it.GetName() == "BLOOM")
				{
					PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 2;
					PSOCreateInfo.GraphicsPipeline.RTVFormats[1] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				}
			}
			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			//Create Vertex Shader
			{
				Managers::AutoVertexShaderDesc VertShaderDesc;
				VertShaderDesc.Name = "PBR_VS";

				Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, VSShader.RawDblPtr(),&LayoutElems);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "PBR_PS";


				std::vector<std::string> defines;

				if (desc.DirLights > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(desc.DirLights)); }
				if (desc.PointLights > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(desc.PointLights)); }
				if (desc.SpotLights > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(desc.SpotLights)); }
				
				for (auto it : desc.mRequiredEffects)
				{
					defines.push_back(it.GetName());
				}
				if (test = true) {
					defines.push_back("TEST");
				}

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PBR.ps.hlsl", defines, std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();
				RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
				Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
				CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSOCreateInfo, Vars, true);


			if (desc.CameraBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

			if (desc.LightsBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_Lights")->Set(desc.LightsBufferPtr);

			if (desc.AnimationBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(desc.AnimationBufferPtr);

			ReflectPixelShaderData();

			mStatus = Graphics::BakeStatus::Baked;
			return true;
		}
	}
}