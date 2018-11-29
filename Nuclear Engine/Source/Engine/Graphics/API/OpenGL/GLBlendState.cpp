#include <Engine/Graphics/API/OpenGL\GLBlendState.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace Graphics
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
				//URGENT: NOT FINSIHED!
				void GLBlendState::Create(GLBlendState* result, const BlendStateDesc& type)
				{
					result->IndependentBlendEnable = type.IndependentBlendEnable;

					for (unsigned int i = 0; i < 8; i++)
					{
						result->targets[i].BlendEnable = type.RenderTarget[i].BlendEnable;
						result->targets[i].SrcBlend = BLEND_TO_GLenum(type.RenderTarget[i].SrcBlend);
						result->targets[i].DestBlend = BLEND_TO_GLenum(type.RenderTarget[i].DestBlend);
						result->targets[i].BlendOp = BLEND_OP_TO_GLenum(type.RenderTarget[i].BlendOp);
						result->targets[i].SrcBlendAlpha = BLEND_TO_GLenum(type.RenderTarget[i].SrcBlendAlpha);
						result->targets[i].DestBlendAlpha = BLEND_TO_GLenum(type.RenderTarget[i].DestBlendAlpha);
						result->targets[i].BlendOpAlpha = BLEND_OP_TO_GLenum(type.RenderTarget[i].BlendOpAlpha);
						result->targets[i].RenderTargetWriteMask = type.RenderTarget[i].RenderTargetWriteMask;
					}
				}
				void GLBlendState::Delete(GLBlendState * result)
				{

				}
				void GLBlendState::Bind(Graphics::Color blendfactor, unsigned int samplemask)
				{
					if (IndependentBlendEnable == false)
					{
						for (unsigned int i = 0; i < 8; i++)
						{
							if (targets[i].BlendEnable)
							{
								glEnablei(GL_BLEND, i);
								glBlendFuncSeparatei(i, targets[0].SrcBlend, targets[0].DestBlend, targets[0].SrcBlendAlpha, targets[0].DestBlendAlpha);
								glBlendEquationSeparatei(i, targets[0].BlendOp, targets[0].BlendOpAlpha);
								glBlendColor(blendfactor.r, blendfactor.g, blendfactor.b, blendfactor.a);
							}
							else
							{
								glDisablei(GL_BLEND, i);
							}

						}			
					}
					else
					{
						if (targets[0].BlendEnable)
						{
							glEnable(GL_BLEND);
							glBlendFuncSeparate(targets[0].SrcBlend, targets[0].DestBlend, targets[0].SrcBlendAlpha, targets[0].DestBlendAlpha);
							glBlendEquationSeparate(targets[0].BlendOp, targets[0].BlendOpAlpha);
							glBlendColor(blendfactor.r, blendfactor.g, blendfactor.b, blendfactor.a);
						}
						else
						{
							glDisable(GL_BLEND);
						}
					}
				}
				BLEND GLBlendState::GLenum_TO_BLEND(GLenum blend)
				{
					switch (blend)
					{
					case GL_ZERO:
						return BLEND::ZERO;
					case GL_ONE: 
						return BLEND::ONE;
					case GL_SRC_COLOR:
						return BLEND::SRC_COLOR;
					case GL_ONE_MINUS_SRC_COLOR: 
						return BLEND::INV_SRC_COLOR;
					case GL_SRC_ALPHA: 
						return BLEND::SRC_ALPHA;
					case GL_ONE_MINUS_SRC_ALPHA:
						return BLEND::INV_SRC_ALPHA;
					case GL_DST_ALPHA:
						return BLEND::DEST_ALPHA;
					case GL_ONE_MINUS_DST_ALPHA: 
						return BLEND::INV_DEST_ALPHA;
					case GL_DST_COLOR: 
						return BLEND::DEST_COLOR;
					case GL_ONE_MINUS_DST_COLOR:
						return BLEND::INV_DEST_COLOR;
					case GL_SRC_ALPHA_SATURATE:
						return BLEND::SRC_ALPHA_SAT;
					case GL_CONSTANT_COLOR: 
						return BLEND::BLEND_FACTOR;
					case GL_ONE_MINUS_CONSTANT_COLOR:
						return BLEND::INV_BLEND_FACTOR;
					case GL_SRC1_COLOR:
						return BLEND::SRC1_COLOR;
					case GL_ONE_MINUS_SRC1_COLOR:
						return BLEND::INV_SRC1_COLOR;
					case GL_SRC1_ALPHA:
						return BLEND::SRC1_ALPHA;
					case GL_ONE_MINUS_SRC1_ALPHA:
						return BLEND::INV_SRC1_ALPHA;
					}

					return BLEND::ONE;
				}
				BLEND_OP GLBlendState::GLenum_TO_BLEND_OP(GLenum blend)
				{
					switch (blend)
					{
					case GL_FUNC_ADD:
						return BLEND_OP::OP_ADD;
					case GL_FUNC_SUBTRACT:
						return BLEND_OP::OP_SUBTRACT;
					case GL_FUNC_REVERSE_SUBTRACT:
						return BLEND_OP::OP_REV_SUBTRACT;
					case GL_MIN:
						return BLEND_OP::OP_MIN;
					case GL_MAX:
						return BLEND_OP::OP_MAX;
					}
					return BLEND_OP::OP_ADD;
				}
				GLenum GLBlendState::BLEND_TO_GLenum(BLEND blend)
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

					return GL_ONE;
				}
				GLenum GLBlendState::BLEND_OP_TO_GLenum(BLEND_OP blend)
				{
					switch (blend)
					{
					case BLEND_OP::OP_ADD: return GL_FUNC_ADD; //1,
					case BLEND_OP::OP_SUBTRACT: return GL_FUNC_SUBTRACT; //2,
					case BLEND_OP::OP_REV_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT; //3,
					case BLEND_OP::OP_MIN: return GL_MIN; //4,
					case BLEND_OP::OP_MAX: return GL_MAX; //5,
					}
					return GL_FUNC_ADD;
				}
			}
		}
	}
}
#endif