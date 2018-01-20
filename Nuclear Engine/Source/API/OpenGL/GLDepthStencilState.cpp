#include <API\OpenGL\GLDepthStencilState.h>
#include <API\API_Types.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
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
				case Comparison_Func::LESS: return GL_LESS;
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
				GLboolean depthmask = false;
				GLenum depthfunc = GL_LESS;

				GLubyte facesmask;
			}
			GLDepthStencilState::~GLDepthStencilState()
			{
				GLboolean depthmask = false;
				GLenum depthfunc;

				GLubyte facesmask;
			}
			void GLDepthStencilState::Create(GLDepthStencilState* result,DepthStencilStateDesc* type)
			{
				if (type->DepthStencilEnabled == true)
				{
					result->depthmask = type->DepthMaskEnabled;
					result->depthfunc = GetGLComparisonFunc(type->DepthFunc);

				}
			}
			void GLDepthStencilState::Bind()
			{
				//glDepthMask(depthmask);
				glDepthFunc(depthfunc);

				//glEnable(GL_STENCIL_TEST);
				//glStencilMask(facesmask);

			}
			void GLDepthStencilState::Unbind()
			{
				//glDepthMask(GL_FALSE);
				glDepthFunc(GL_LESS);
			}
		}
	}
}

#endif