#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace API
	{
		struct DepthStencilStateDesc;
		namespace OpenGL
		{
			namespace Internals
			{
				struct Stencil_Face_Desc {
					GLenum StencilFailOp;
					GLenum StencilDepthFailOp;
					GLenum StencilPassOp;
					GLenum StencilFunc;
				};
			}

			class NEAPI GLDepthStencilState
			{
			public:
				GLDepthStencilState();
				~GLDepthStencilState();

				static void Create(GLDepthStencilState *state, const DepthStencilStateDesc& type);
				static void Delete(GLDepthStencilState *state);

				void Bind();
			protected:
				GLboolean depthmask;
				GLenum depthfunc;
				bool depthenabled;

				bool stencilenabled;
				GLuint stencilwritemask;
				GLuint stencilreadmask;

			};
		}
	}
}

#endif