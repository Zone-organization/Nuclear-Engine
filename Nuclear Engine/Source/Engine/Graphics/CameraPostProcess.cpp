#include <Engine\Graphics\CameraPostProcess.h>
#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/include/CommonlyUsedStates.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		void CameraPostProcess::Bake(const PostProcessDesc& Desc)
		{
			Assets::Mesh::CreateScreenQuad(&PPScreenQuad);
			CreateRenderTarget(Desc);
			CreatePipeline(Desc);



			//LLGL::SamplerDesc Samplerdesc;
			//Samplerdesc.Filter = LLGL::TextureFilter::Point2D;
			//LLGL::Sampler::Create(&ScreenSampler, Samplerdesc);
		}
		void CameraPostProcess::CreateRenderTarget(const PostProcessDesc& Desc)
		{
			RenderTargetDesc RTDesc;
			RTDesc.Width = Desc.Width;
			RTDesc.Height = Desc.Height;

			if (Desc.HDR == true)
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
			else
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA8_UNORM;


			PostProcessRT.Create(RTDesc);
		}
		void CameraPostProcess::CreatePipeline(const PostProcessDesc& Desc)
		{
			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;
			VertShaderDesc.Name = "CameraPP_VS";

			std::vector<LayoutElement> LayoutElems;
			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;


			VShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, &LayoutElems);

			ShaderCreateInfo CreationAttribs;
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
			CreationAttribs.Desc.Name = "CameraPP_PS";
			
			std::vector<std::string> defines;

			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION_ENABLED"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }

			CreationAttribs.Source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl", defines, std::vector<std::string>(), true).c_str();
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PShader);



			PipelineStateDesc PSODesc;

			PSODesc.Name = "CameraPP_PSO";
			PSODesc.IsComputePipeline = false;
			PSODesc.GraphicsPipeline.NumRenderTargets = 1;
			PSODesc.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSODesc.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSODesc.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSODesc.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSODesc.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSODesc.GraphicsPipeline.pVS = VShader;
			PSODesc.GraphicsPipeline.pPS = PShader;
			PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSODesc.GraphicsPipeline.InputLayout.NumElements = LayoutElems.size();
			PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

			std::vector<ShaderResourceVariableDesc> Vars;
			Vars.push_back({ SHADER_TYPE_PIXEL, "ScreenTex", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });
		
			std::vector<StaticSamplerDesc> StaticSamplers;
			StaticSamplers.push_back({ SHADER_TYPE_PIXEL, "ScreenTex", Sam_LinearClamp });


			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPSO, PSODesc, Vars, false, StaticSamplers);

		}
	}
}