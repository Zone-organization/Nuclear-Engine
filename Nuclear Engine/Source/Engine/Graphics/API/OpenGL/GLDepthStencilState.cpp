#include <Engine/Graphics/API/OpenGL\GLDepthStencilState.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/RenderStates_Types.h>


namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{

				GLenum GetGLComparisonFunc(Comparison_Func func)
				{
					switch (func)
					{
					case Comparison_Func::NEVER: return GL_NEVER;
					case Comparison_Func::LESS:	return GL_LESS;
					case Comparison_Func::EQUAL: return GL_EQUAL;
					case Comparison_Func::LESS_EQUAL: return GL_LEQUAL;
					case Comparison_Func::GREATER: return GL_GREATER;
					case Comparison_Func::NOT_EQUAL: return GL_NOTEQUAL;
					case Comparison_Func::GREATER_EQUAL: return GL_GEQUAL;
					case Comparison_Func::ALWAYS: return GL_ALWAYS;

					default: return GL_ALWAYS;
					}
				}

				GLDepthStencilState::GLDepthStencilState()
				{
					mDepthEnabled = false;
					mStencilEnabled = false;

				}
				GLDepthStencilState::~GLDepthStencilState()
				{
					mDepthEnabled = false;
					mStencilEnabled = false;
				}
				void GLDepthStencilState::Create(GLDepthStencilState* result, const DepthStencilStateDesc& type)
				{
					result->mDepthEnabled = type.DepthEnabled;
					result->mStencilEnabled = type.StencilEnabled;

					if (type.DepthEnabled == true)
					{
						result->mDepthMask = type.DepthMaskEnabled;
						result->mDepthFunc = GetGLComparisonFunc(type.DepthFunc);
						result->mDepthEnabled = true;
					}
					//URGENT: NOT FINSIHED!
					if (type.StencilEnabled == true)
					{
						result->mStencilEnabled = true;
						result->mStencilWriteMask = type.StencilWriteMask;
						result->mStencilReadMask = type.StencilReadMask;

					}
				}
				void GLDepthStencilState::Delete(GLDepthStencilState * result)
				{
					result->mDepthMask = false;
					result->mDepthFunc = GL_LESS;
					result->mDepthEnabled = false;
				}
				void GLDepthStencilState::Bind()
				{
					if (mDepthEnabled)
					{
						glEnable(GL_DEPTH_TEST);
						glDepthMask(mDepthMask);
						glDepthFunc(mDepthFunc);
					}
					else {
						glDisable(GL_DEPTH_TEST);
					}
					/*	if (mStencilEnabled)
						{
							glEnable(GL_STENCIL_TEST);
						//	glStencilMaskSeparate(0xFF); // each bit is written to the stencil buffer as is
						//	glStencilMaskSeparate(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

						}
						else {
							glDisable(GL_STENCIL_TEST);

						}*/

				}
			}
		}
	}
}
#endif