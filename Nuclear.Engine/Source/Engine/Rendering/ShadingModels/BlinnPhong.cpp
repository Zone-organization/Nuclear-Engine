#include <Engine\Rendering\ShadingModels\BlinnPhong.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>

namespace Nuclear
{
	namespace Rendering
	{
		BlinnPhong::BlinnPhong()
		{
			static int i = 0;
			mName = "NE_BlinnPhong" + std::to_string(i);

			i++;

			mID = Utilities::Hash(mName);

			mRenderingEffects[Utilities::Hash("BLOOM")] = ShaderEffect("BLOOM", ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}
		bool BlinnPhong::Bake(const ShadingModelBakingDesc& desc)
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = TEX_FORMAT_RGBA8_UNORM;
			//Check for bloom
			for (auto it : desc.mRequiredEffects)
			{
				if (it.GetName() == "BLOOM")
				{
					PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 2;
					PSOCreateInfo.GraphicsPipeline.RTVFormats[1] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				}
			}


			PSOCreateInfo.PSODesc.Name = "BlinnPhongPSO";

			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			if (mInitInfo.mDefferedPipeline)
			{
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			}

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;

			std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(mInitInfo.mDefferedPipeline);

			//Create Vertex Shader
			{

				ShaderCreateInfo CreationAttribs;

				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "BlinnPhongVS";


				std::vector<std::string> defines;
				if (mInitInfo.mDefferedPipeline)
				{
					defines.push_back("NE_DEFFERED");
				}
				if (mInitInfo.ShadowingEnabled == true)
				{
					defines.push_back("NE_SHADOWS");
					if (desc.Max_DirLight_Caster > 0) { defines.push_back("NE_MAX_DIR_CASTERS " + std::to_string(desc.Max_DirLight_Caster)); }
					if (desc.Max_SpotLight_Caster > 0) { defines.push_back("NE_MAX_SPOT_CASTERS " + std::to_string(desc.Max_SpotLight_Caster)); }
				}
				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Basic.vs.hlsl", defines, std::vector<std::string>(), true);
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
				CreationAttribs.Desc.Name = "BlinnPhongPS";


				std::vector<std::string> defines;

				if (desc.DirLights > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(desc.DirLights)); }
				if (desc.PointLights > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(desc.PointLights)); }
				if (desc.SpotLights > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(desc.SpotLights)); }

				for (auto it : desc.mRequiredEffects)
				{
					defines.push_back(it.GetName());
				}
				if (mInitInfo.mDefferedPipeline)
				{
					defines.push_back("NE_DEFFERED");
				}
				if (mInitInfo.ShadowingEnabled == true)
				{
					defines.push_back("NE_SHADOWS");
					if (desc.Max_DirLight_Caster > 0) { defines.push_back("NE_MAX_DIR_CASTERS " + std::to_string(desc.Max_DirLight_Caster)); }
					if (desc.Max_SpotLight_Caster > 0) { defines.push_back("NE_MAX_SPOT_CASTERS " + std::to_string(desc.Max_SpotLight_Caster)); }
				}
				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/BlinnPhong.ps.hlsl", defines, std::vector<std::string>(), true);
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

			if (desc.LightsBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_Lights")->Set(desc.LightsBufferPtr);

			if (mInitInfo.ShadowingEnabled == true)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_ShadowCasters")->Set(desc.ShadowCastersBufferPtr);

			if (!mInitInfo.mDefferedPipeline)
			{
				if (desc.CameraBufferPtr)
					mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

				if (desc.AnimationBufferPtr)
					mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(desc.AnimationBufferPtr);

				ReflectPixelShaderData();
			}
			else {
				BakeGBufferPipeline(desc);
			}
			mStatus = Graphics::BakeStatus::Baked;

			return true;
		}
		std::vector<Graphics::RenderTargetDesc> BlinnPhong::GetGBufferDesc()
		{
			std::vector<Graphics::RenderTargetDesc> result = {
				Graphics::RenderTargetDesc(TEX_FORMAT_RGBA16_FLOAT,TEX_FORMAT_UNKNOWN, "Position"),
				Graphics::RenderTargetDesc(TEX_FORMAT_RGBA16_FLOAT,TEX_FORMAT_UNKNOWN, "Normal"),
				Graphics::RenderTargetDesc(TEX_FORMAT_RGBA8_UNORM, TEX_FORMAT_UNKNOWN, "Albedo")
			};
			return result;
		}
		void BlinnPhong::BakeGBufferPipeline(const ShadingModelBakingDesc& desc)
		{
	        std::vector<LayoutElement> Layout;

            Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));  //POS
            Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
            Layout.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
            Layout.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
			Layout.push_back(LayoutElement(4, 0, 3, VT_FLOAT32, false));  //Bitangents
            Layout.push_back(LayoutElement(5, 0, 4, VT_INT32, false));    //BONE ID
            Layout.push_back(LayoutElement(6, 0, 4, VT_FLOAT32, false));  //WEIGHT


            GraphicsPipelineStateCreateInfo PSOCreateInfo;
			PSOCreateInfo.PSODesc.Name = "GBufferPipelinePSO";

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
                CreationAttribs.Desc.Name = "GBUFFER_VS";

                auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/GBuffer.vs.hlsl");
                CreationAttribs.Source = source.c_str();

                Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VSShader);
            }

            //Create Pixel Shader
            {
                ShaderCreateInfo CreationAttribs;
                CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
                CreationAttribs.UseCombinedTextureSamplers = true;
                CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
                CreationAttribs.EntryPoint = "main";
                CreationAttribs.Desc.Name = "GBUFFER_PS";

                auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/GBuffer.ps.hlsl");
                CreationAttribs.Source = source.c_str();

                Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
            }

            PSOCreateInfo.pVS = VSShader;
            PSOCreateInfo.pPS = PSShader;

            PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 3;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = TEX_FORMAT_RGBA16_FLOAT;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[1] = TEX_FORMAT_RGBA16_FLOAT;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[2] = TEX_FORMAT_RGBA8_UNORM;
            //PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
            PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_D32_FLOAT;
            PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
            PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
            PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
            PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
            PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

            auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
            Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mGBufferPipeline, PSOCreateInfo, Vars, true);

			mGBufferPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

			mGBufferPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(desc.AnimationBufferPtr);

			ReflectPixelShaderData();
		}
	}
}