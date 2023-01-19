#pragma once
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Diligent
{
	constexpr auto serialize(auto& archive, BlendStateDesc& self)
	{
		//TODO: RenderTargetBlendDesc RenderTargets[DILIGENT_MAX_RENDER_TARGETS];
		return archive(self.AlphaToCoverageEnable, self.IndependentBlendEnable);
	}

	constexpr auto serialize(auto& archive, RasterizerStateDesc& self)
	{
		return archive(self.FillMode
			, self.CullMode
			, self.FrontCounterClockwise
			, self.DepthClipEnable
			, self.ScissorEnable
			, self.AntialiasedLineEnable
			, self.DepthBias
			, self.DepthBiasClamp
			, self.SlopeScaledDepthBias);
	}

	constexpr auto serialize(auto& archive, DepthStencilStateDesc& self)
	{
		 //TODO: StencilOpDesc FrontFace;	StencilOpDesc BackFace;
		return archive(self.DepthEnable
			, self.DepthWriteEnable
			, self.DepthFunc
			, self.StencilEnable
			, self.StencilReadMask
			, self.StencilWriteMask);
	}

	constexpr auto serialize(auto& archive, GraphicsPipelineDesc& self)
	{
		//TODO:     InputLayoutDesc InputLayout;     SampleDesc SmplDesc;

		return archive(self.BlendDesc
			,self.SampleMask
			, self.RasterizerDesc
			, self.DepthStencilDesc
			, self.NumViewports
			, self.NumRenderTargets
			, self.SubpassIndex
			, self.ShadingRateFlags
			, self.RTVFormats
			, self.DSVFormat);
	}
}