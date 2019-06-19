#include <Engine\Graphics\RenderingPipelines\DiffuseOnly.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>


namespace NuclearEngine
{
	namespace Graphics
	{
		bool DiffuseOnly::Bake(const RenderingPipelineDesc& desc)
		{
			PipelineStateDesc PSODesc;

			PSODesc.Name = "RenderSystem PSO";
			PSODesc.IsComputePipeline = false;
			PSODesc.GraphicsPipeline.NumRenderTargets = 1;
			PSODesc.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSODesc.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSODesc.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSODesc.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSODesc.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			//Create Vertex Shader
			{
				Managers::AutoVertexShaderDesc VertShaderDesc;
				VertShaderDesc.Name = "DiffuseOnlyVS";
				VertShaderDesc.OutFragPos = false;
				VSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, &LayoutElems);
			}



			//Create Pixel Shader
			{
				Managers::AutoPixelShaderDesc PixelShaderDesc;
				PixelShaderDesc.Name = "DiffuseOnlyPS";
				PSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoPixelShader(PixelShaderDesc);
			}

			PSODesc.GraphicsPipeline.pVS = VSShader;
			PSODesc.GraphicsPipeline.pPS = PSShader;
			PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSODesc.GraphicsPipeline.InputLayout.NumElements = LayoutElems.size();
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSODesc, Vars, true);

			return true;
			return false;
		}
	}
}