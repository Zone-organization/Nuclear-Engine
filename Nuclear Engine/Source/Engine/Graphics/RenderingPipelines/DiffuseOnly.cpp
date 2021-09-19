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
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "DiffuseOnly PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			//Create Vertex Shader
			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Name = "DiffuseOnlyVS";
			VertShaderDesc.OutFragPos = false;
			VertShaderDesc.InTangents = true;
			VSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, &LayoutElems);

			//Create Pixel Shader
			Managers::AutoPixelShaderDesc PixelShaderDesc;
			PixelShaderDesc.Name = "DiffuseOnlyPS";
			PSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoPixelShader(PixelShaderDesc);

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSOCreateInfo, Vars, true);

			if (desc.CameraBufferPtr)
				mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(desc.CameraBufferPtr);

			ReflectPixelShaderData();
			mStatus = BakeStatus::Baked;

			return true;
		}
	}
}