#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
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