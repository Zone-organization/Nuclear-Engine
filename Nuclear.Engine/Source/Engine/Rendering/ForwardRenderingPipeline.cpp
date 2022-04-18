#include "Engine/Rendering/ForwardRenderingPipeline.h"
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Rendering
	{
		ForwardRenderingPipeline::ForwardRenderingPipeline(const std::string& name)
			: RenderingPipeline(name)
		{
			mPostProcessingEffects[Utilities::Hash("HDR")] = Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("GAMMACORRECTION")] = Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::PostProcessingEffect, false);
			mPostProcessingEffects[Utilities::Hash("BLOOM")] = Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::PostProcessingAndRenderingEffect, false);
		}
		void ForwardRenderingPipeline::Bake(const RenderingPipelineBakingDesc& desc)
		{
			ShadingModelBakingDesc SMDesc = desc.mShadingModelDesc;
			for (auto it : mPairedEffects)
			{
				SMDesc.mRequiredEffects.push_back(it.second);
			}

			GetShadingModel()->Bake(SMDesc);

			mRTWidth = desc.mRTWidth;
			mRTHeight = desc.mRTHeight;
			BakeRenderTarget();
			BakePipeline();
		}


		void ForwardRenderingPipeline::BakeRenderTarget()
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = mRTWidth;
			RTDesc.Height = mRTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			SceneRT.Create(RTDesc);
		}
		void ForwardRenderingPipeline::BakePipeline()
		{
			std::vector<LayoutElement> Layout;

			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::CompoundPipelineDesc PSOCreateInfo;

			for (auto it : mPostProcessingEffects)
			{
				PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.second.GetName()));
			}

			PSOCreateInfo.mVShaderPath = VS_Path;
			PSOCreateInfo.mPShaderPath = PS_Path;

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

			mPipeline.Create(PSOCreateInfo);

			bool bloomincluded = false;
			for (auto it : mPostProcessingEffects)
			{
				if (it.second.GetName() == "BLOOM")
				{
					bloomincluded = true;
				}
			}

			//Create Blur pipeline
			if (bloomincluded)
			{
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.Width = mRTWidth;
				RTDesc.Height = mRTHeight;
				RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
				RTDesc.mCreateDepth = false;

				BloomRT.Create(RTDesc);

				mBloomBlur.Initialize(mRTWidth, mRTHeight);
			}

			UpdatePSO(true);
		}

	}
}