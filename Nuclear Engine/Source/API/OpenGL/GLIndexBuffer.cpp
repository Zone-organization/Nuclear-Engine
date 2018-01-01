#include <API\OpenGL\GLIndexBuffer.h>

#ifdef NE_COMPILE_OPENGL3_3

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
					glDeleteBuffers(1, &buffer);
				}
				buffer = 0;
			}

			void GLIndexBuffer::Create(GLIndexBuffer* result, const void * indices, unsigned int count)
			{
				glGenBuffers(1, &result->buffer);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffer);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			void GLIndexBuffer::Bind()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
			}
		}
	}
}

#endif