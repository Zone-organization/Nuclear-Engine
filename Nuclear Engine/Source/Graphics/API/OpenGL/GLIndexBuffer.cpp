#include <Graphics/API/OpenGL\GLIndexBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLError.h>

namespace NuclearEngine
{
	namespace Graphics
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
					buffer = 0;
				}

				void GLIndexBuffer::Create(GLIndexBuffer* result, const void * indices, unsigned int count)
				{
					GLCall(glGenBuffers(1, &result->buffer));

					GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffer));

					GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

					GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
				}

				void GLIndexBuffer::Delete(GLIndexBuffer * ibuffer)
				{
					if (ibuffer->buffer != 0)
					{
						GLCall(glDeleteBuffers(1, &ibuffer->buffer));
					}
					ibuffer->buffer = 0;
				}

				void GLIndexBuffer::Bind()
				{
					GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
				}
			}
		}
	}
}

#endif