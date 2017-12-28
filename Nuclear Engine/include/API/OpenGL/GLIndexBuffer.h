#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			class NEAPI GLIndexBuffer
			{
			public:
				GLIndexBuffer();
				~GLIndexBuffer();

				static GLIndexBuffer Create(const void* indices, unsigned int count);

				void Bind();
				void Unbind();
			private:
				GLuint buffer;
			};
		}
	}
}
#endif