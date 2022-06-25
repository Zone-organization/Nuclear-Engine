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
			RefCntAutoPtr<IShader> HorzVShader;
			RefCntAutoPtr<IShader> VertVShader;

			RefCntAutoPtr<IShader> PShader;
			std::vector<LayoutElement> LayoutElems;
			LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
			LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false)); //UV
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.Desc.Name = "HorzBlurVS";
				CreationAttribs.EntryPoint = "main";
				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.vs.hlsl", { "HORIZENTAL" }, std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &HorzVShader);
			}
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.Desc.Name = "VertBlurVS";
				CreationAttribs.EntryPoint = "main";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.vs.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VertVShader);
			}
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.Desc.Name = "BlurPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.ps.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PShader);
			}
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_UNKNOWN;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.pVS = HorzVShader;
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

			PSOCreateInfo.PSODesc.Name = "HorzBlurPSO";
			Graphics::Context::GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mHorzBlurPSO);

			PSOCreateInfo.pVS = VertVShader;
			PSOCreateInfo.PSODesc.Name = "VertBlurPSO";
			Graphics::Context::GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mVertBlurPSO);

			BufferDesc CBDesc;
			CBDesc.Name = "BlurCB";
			CBDesc.Size = sizeof(Math::Vector4i);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mBlurCB);

			mHorzBlurPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "ScreenSizeBuffer")->Set(mBlurCB);
			mVertBlurPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "ScreenSizeBuffer")->Set(mBlurCB);

			mHorzBlurPSO->CreateShaderResourceBinding(&mHorzBlurSRB, true);
			mVertBlurPSO->CreateShaderResourceBinding(&mVertBlurSRB, true);

			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			BlurHorizentalRT.Create(RTDesc);
			BlurVerticalRT.Create(RTDesc);
		}
		void BlurEffect::SetHorizentalPSO(ITextureView* texture)
		{
			Graphics::Context::GetContext()->SetPipelineState(mHorzBlurPSO);

			Graphics::Context::GetContext()->SetRenderTargets(1, BlurHorizentalRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			mHorzBlurSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(texture);
		}
		void BlurEffect::SetVerticalPSO(ITextureView* texture)
		{
			Graphics::Context::GetContext()->SetPipelineState(mVertBlurPSO);

			Graphics::Context::GetContext()->SetRenderTargets(1, BlurVerticalRT.GetRTVDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			mVertBlurSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(texture);
		}
		void BlurEffect::ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight)
		{
			//mHorzBlurSRB.Release();
			//mVertBlurSRB.Release();
			//mHorzBlurPSO->CreateShaderResourceBinding(&mHorzBlurSRB, true);
			//mVertBlurPSO->CreateShaderResourceBinding(&mVertBlurSRB, true);

			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			BlurHorizentalRT.Create(RTDesc);
			BlurVerticalRT.Create(RTDesc);
		}
	}
}