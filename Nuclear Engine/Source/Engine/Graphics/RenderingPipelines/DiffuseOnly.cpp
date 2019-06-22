#include <Engine\Graphics\RenderingPipelines\DiffuseOnly.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include <Base/Utilities/Hash.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		DiffuseOnly::DiffuseOnly()
		{
			mID  = Utilities::Hash("NE_Diffuse_Only");
		}

		bool DiffuseOnly::Bake(const RenderingPipelineDesc& desc)
		{
			PipelineStateDesc PSODesc;

			PSODesc.Name = "DiffuseOnly PSO";
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

			if (desc.CameraBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

			ReflectPixelShaderData();

			return true;
		}
	}
}