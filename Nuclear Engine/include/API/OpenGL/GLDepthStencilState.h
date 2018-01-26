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
			class NEAPI GLDepthStencilState
			{
			public:
				GLDepthStencilState();
				~GLDepthStencilState();

				static void Create(GLDepthStencilState *state, const DepthStencilStateDesc& type);
				static void Delete(GLDepthStencilState *state);

				void Bind();

				static void Bind_Default();
			protected:
				GLboolean depthmask;
				GLenum depthfunc;
				bool depthenabled;

			};
		}
	}
}

#endif