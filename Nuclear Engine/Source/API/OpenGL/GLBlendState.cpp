#include <API\OpenGL\GLBlendState.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\RenderStates_Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			GLBlendState::GLBlendState()
			{

			}
			GLBlendState::~GLBlendState()
			{

			}
			GLenum Blend2_GLenumBlend(BLEND blend)
			{
				switch (blend)
				{
				case BLEND::ZERO: return GL_ZERO; //1,
				case BLEND::ONE: return GL_ONE; //2,
				case BLEND::SRC_COLOR: return GL_SRC_COLOR; //3,
				case BLEND::INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR; //4,
				case BLEND::SRC_ALPHA: return GL_SRC_ALPHA; //5,
				case BLEND::INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA; //6,
				case BLEND::DEST_ALPHA: return GL_DST_ALPHA; //7,
				case BLEND::INV_DEST_ALPHA: return GL_ONE_MINUS_DST_ALPHA; //8,
				case BLEND::DEST_COLOR: return GL_DST_COLOR; //9,
				case BLEND::INV_DEST_COLOR: return GL_ONE_MINUS_DST_COLOR; //10,
				case BLEND::SRC_ALPHA_SAT: return GL_SRC_ALPHA_SATURATE; //11,
				case BLEND::BLEND_FACTOR: return GL_CONSTANT_COLOR; //14,
				case BLEND::INV_BLEND_FACTOR: return GL_ONE_MINUS_CONSTANT_COLOR; //15,
				case BLEND::SRC1_COLOR: return GL_SRC1_COLOR; //16,
				case BLEND::INV_SRC1_COLOR: return GL_ONE_MINUS_SRC1_COLOR; //17,
				case BLEND::SRC1_ALPHA: return GL_SRC1_ALPHA; //18,
				case BLEND::INV_SRC1_ALPHA: return GL_ONE_MINUS_SRC1_ALPHA; //19
				}
			}
			GLenum BlendOP2_GLenumBlendOP(BLEND_OP blend)
			{
				switch (blend)
				{
				case BLEND_OP::OP_ADD: return GL_FUNC_ADD; //1,
				case BLEND_OP::OP_SUBTRACT: return GL_FUNC_SUBTRACT; //2,
				case BLEND_OP::OP_REV_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT; //3,
				case BLEND_OP::OP_MIN: return GL_MIN; //4,
				case BLEND_OP::OP_MAX: return GL_MAX; //5,
				}
			}
			//URGENT: NOT FINSIHED!
			void GLBlendState::Create(GLBlendState* result, const BlendStateDesc& type)
			{
				result->IndependentBlendEnable = type.IndependentBlendEnable;

				for (unsigned int i = 0; i < 8; i++)
				{
					result->targets[i].BlendEnable = type.RenderTarget[i].BlendEnable;
					result->targets[i].SrcBlend = Blend2_GLenumBlend(type.RenderTarget[i].SrcBlend);
					result->targets[i].DestBlend = Blend2_GLenumBlend(type.RenderTarget[i].DestBlend);
					result->targets[i].BlendOp = BlendOP2_GLenumBlendOP(type.RenderTarget[i].BlendOp);
					result->targets[i].SrcBlendAlpha = Blend2_GLenumBlend(type.RenderTarget[i].SrcBlendAlpha);
					result->targets[i].DestBlendAlpha = Blend2_GLenumBlend(type.RenderTarget[i].DestBlendAlpha);
					result->targets[i].BlendOpAlpha = BlendOP2_GLenumBlendOP(type.RenderTarget[i].BlendOpAlpha);
					result->targets[i].RenderTargetWriteMask = type.RenderTarget[i].RenderTargetWriteMask;
				}
			}
			void GLBlendState::Delete(GLBlendState * result)
			{
			
			}
			void GLBlendState::Bind()
			{
				if (IndependentBlendEnable == false)
				{
					if (targets[0].BlendEnable)
					{
						glEnable(GL_BLEND);
					}
					else 
					{
						glDisable(GL_BLEND);
					}
				}

			}
			void GLBlendState::Bind_Default()
			{

			}
		}
	}
}

#endif