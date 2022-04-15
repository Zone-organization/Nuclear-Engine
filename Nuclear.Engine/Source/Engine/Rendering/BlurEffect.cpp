#include "Engine/Rendering/BlurEffect.h"
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include <Diligent/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>

namespace Nuclear
{
	namespace Rendering
	{
		void BlurEffect::Initialize(Uint32 RTWidth, Uint32 RTHeight)
		{
			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;
			std::vector<LayoutElement> LayoutElems;

			{
				LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
				LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false)); //UV

				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.Desc.Name = "BlurVS";
				CreationAttribs.EntryPoint = "main";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/BasicVertex.vs.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VShader);
			}
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.Desc.Name = "BlurPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PShader);
			}
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "BlurPSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_UNKNOWN;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

			std::vector<ShaderResourceVariableDesc> Vars;
			Vars.push_back({ SHADER_TYPE_PIXEL, "Texture", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC });

			std::vector<ImmutableSamplerDesc> StaticSamplers;
			StaticSamplers.push_back({ SHADER_TYPE_PIXEL, "Texture", Sam_LinearClamp });

			PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = static_cast<Uint32>(Vars.size());
			PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars.data();
			PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = static_cast<Uint32>(StaticSamplers.size());
			PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = StaticSamplers.data();

			Graphics::Context::GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mBlurPSO);

			BufferDesc CBDesc;
			CBDesc.Name = "BlurCB";
			CBDesc.Size = sizeof(Math::Vector4i);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mBlurCB);

			mBlurPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "BlurOptions")->Set(mBlurCB);

			mBlurPSO->CreateShaderResourceBinding(&mBlurSRB, true);

			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
			RTDesc.mCreateDepth = false;

		//	mBlurPSO.Create(RTDesc);

			BlurHorizentalRT.Create(RTDesc);
			BlurVerticalRT.Create(RTDesc);
		}
	}
}