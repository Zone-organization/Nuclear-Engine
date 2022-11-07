#include <Engine\Rendering\ShadingModels\DiffuseOnly.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include <Core/Utilities/Hash.h>

namespace Nuclear
{
	namespace Rendering
	{
		DiffuseOnly::DiffuseOnly()
			: ShadingModel()
		{
			mID  = Utilities::Hash("NE_Diffuse_Only");
			mName = "NE_Diffuse_Only";
		}

		bool DiffuseOnly::Bake(const ShadingModelBakingDesc& desc)
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "DiffuseOnly PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = !COORDSYSTEM_LH_ENABLED;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

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
				CreationAttribs.Desc.Name = "DiffuseOnlyVS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Basic.vs.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
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
				CreationAttribs.Desc.Name = "DiffuseOnlyPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/DiffuseOnly.ps.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, PSShader.RawDblPtr());
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);

			//Create normal pipeline
			{
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSOCreateInfo, Vars, true);

				if (desc.CameraBufferPtr)
					mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

				if (desc.AnimationBufferPtr)
					mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(desc.AnimationBufferPtr);
			}

			//create skinned pipeline
			{		
				RefCntAutoPtr<IShader> SkinnedVShader;
				//Create Skinned Vertex Shader
				{

					ShaderCreateInfo CreationAttribs;

					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "DiffuseOnlyVS";

					std::vector<std::string> defines;
					defines.push_back("NE_DISABLE_ANIMATION");

					auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Basic.vs.hlsl", std::vector<std::string>(), defines, true);
					CreationAttribs.Source = source.c_str();
					RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
					Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
					CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;
					Graphics::Context::GetDevice()->CreateShader(CreationAttribs, SkinnedVShader.RawDblPtr());
				}

				PSOCreateInfo.pVS = SkinnedVShader;
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mSkinnedPipeline, PSOCreateInfo, Vars, true);

				if (desc.CameraBufferPtr)
					mSkinnedPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

				if (desc.AnimationBufferPtr)
					mSkinnedPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(desc.AnimationBufferPtr);
			}

			ReflectPixelShaderData();
			mStatus = Graphics::BakeStatus::Baked;

			return true;
		}
	}
}