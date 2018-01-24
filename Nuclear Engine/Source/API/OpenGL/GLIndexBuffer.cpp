#include <API\OpenGL\GLIndexBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\OpenGL\GLError.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			GLIndexBuffer::GLIndexBuffer() : buffer(0)
			{
			}
			GLIndexBuffer::~GLIndexBuffer()
			{
				if (buffer != 0)
				{
					GLCall(glDeleteBuffers(1, &buffer));
				}
				buffer = 0;
			}

			void GLIndexBuffer::Create(GLIndexBuffer* result, const void * indices, unsigned int count)
			{
				GLCall(glGenBuffers(1, &result->buffer));

				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffer));

				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}

			void GLIndexBuffer::Bind()
			{
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
			}
		}
	}
}

#endif