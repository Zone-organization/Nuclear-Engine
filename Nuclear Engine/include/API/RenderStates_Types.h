#pragma once

namespace NuclearEngine {
	namespace API {
		enum class Comparison_Func {
			NEVER,
			LESS,
			EQUAL,
			LESS_EQUAL,
			GREATER,
			NOT_EQUAL,
			GREATER_EQUAL,
			ALWAYS
		};
		enum class Stencil_OP
		{
			KEEP,
			ZERO,
			REPLACE,
			INCREASE_SAT,
			DECREASE_SAT,
			INVERT,
			INCREASE,
			DECREASE
		};

		struct Stencil_Face_Desc {
			Stencil_OP StencilFailOp = Stencil_OP::KEEP;
			Stencil_OP StencilDepthFailOp = Stencil_OP::KEEP;
			Stencil_OP StencilPassOp = Stencil_OP::KEEP;
			Comparison_Func StencilFunc = Comparison_Func::NOT_EQUAL;
		};

		struct DepthStencilStateDesc {
			bool DepthEnabled = false;
			bool DepthMaskEnabled = false;
			Comparison_Func DepthFunc = Comparison_Func::LESS;

			bool StencilEnabled = false;
			unsigned char StencilReadMask = 0xFF;
			unsigned char StencilWriteMask = 0xFF;

			Stencil_Face_Desc StencilFrontFace;
			Stencil_Face_Desc StencilBackFace;
		};
		enum class CullMode {
			None = 1,
			Front = 2,
			Back = 3
		};
		enum class FillMode {
			Wireframe = 2,
			Solid = 3
		};
		struct RasterizerStateDesc {
			FillMode FillMode = FillMode::Solid;
			CullMode CullMode = CullMode::None;
			bool FrontCounterClockwise = false;
			int DepthBias = 0;
			float DepthBiasClamp = 0.0f;
			float SlopeScaledDepthBias = 0.0f;
			bool DepthClipEnable = true;
			bool ScissorEnable = false;
			bool MultisampleEnable = false;
			bool AntialiasedLineEnable = false;
		};
	}
}