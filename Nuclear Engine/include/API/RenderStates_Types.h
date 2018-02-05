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
		
		enum class BLEND
		{
			ZERO = 1,
			ONE = 2,
			SRC_COLOR = 3,
			INV_SRC_COLOR = 4,
			SRC_ALPHA = 5,
			INV_SRC_ALPHA = 6,
			DEST_ALPHA = 7,
			INV_DEST_ALPHA = 8,
			DEST_COLOR = 9,
			INV_DEST_COLOR = 10,
			SRC_ALPHA_SAT = 11,
			BLEND_FACTOR = 14,
			INV_BLEND_FACTOR = 15,
			SRC1_COLOR = 16,
			INV_SRC1_COLOR = 17,
			SRC1_ALPHA = 18,
			INV_SRC1_ALPHA = 19
		};

		
		enum class BLEND_OP
		{
			OP_ADD = 1,
			OP_SUBTRACT = 2,
			OP_REV_SUBTRACT = 3,
			OP_MIN = 4,
			OP_MAX = 5
		};

		enum class COLOR_WRITE_ENABLE
		{
			COLOR_WRITE_ENABLE_RED = 1,
			COLOR_WRITE_ENABLE_GREEN = 2,
			COLOR_WRITE_ENABLE_BLUE = 4,
			COLOR_WRITE_ENABLE_ALPHA = 8,
			COLOR_WRITE_ENABLE_ALL = (((COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN) | COLOR_WRITE_ENABLE_BLUE) | COLOR_WRITE_ENABLE_ALPHA)
		};

		typedef struct RENDER_TARGET_BLEND_DESC
		{
			bool BlendEnable;
			BLEND SrcBlend;
			BLEND DestBlend;
			BLEND_OP BlendOp;
			BLEND SrcBlendAlpha;
			BLEND DestBlendAlpha;
			BLEND_OP BlendOpAlpha;
			unsigned char RenderTargetWriteMask;
		} 	RENDER_TARGET_BLEND_DESC;

		struct BlendStateDesc
		{
			bool AlphaToCoverageEnable;
			bool IndependentBlendEnable;
			RENDER_TARGET_BLEND_DESC RenderTarget[8];
		};

	}
}