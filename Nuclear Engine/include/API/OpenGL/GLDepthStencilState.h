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

				static GLDepthStencilState Create(DepthStencilStateDesc type);

				void Bind();

				void Unbind();
			protected:
				unsigned int count;

				GLboolean depthmask = false;
				GLenum depthfunc;

				GLubyte facesmask;

			};
		}
	}
}

#endif