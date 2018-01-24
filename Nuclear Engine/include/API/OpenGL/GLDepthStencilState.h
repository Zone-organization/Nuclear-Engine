#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	struct DepthStencilStateDesc;

	namespace API
	{
		namespace OpenGL
		{
			class NEAPI GLDepthStencilState
			{
			public:
				GLDepthStencilState();
				~GLDepthStencilState();

				static void Create(GLDepthStencilState *state,DepthStencilStateDesc* type);
				static void Delete(GLDepthStencilState *state);

				void Bind();

				void Unbind();
			protected:
				GLboolean depthmask = false;
				GLenum depthfunc;

				GLubyte facesmask;

			};
		}
	}
}

#endif