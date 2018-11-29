#include <Engine/Graphics/API/OpenGL\GLIndexBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/Buffer_Types.h>

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

				void GLIndexBuffer::Create(GLIndexBuffer* result,const IndexBufferDesc& desc)
				{
					glGenBuffers(1, &result->buffer);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffer);

					GLenum Usage;
					if (desc.UsageType == BufferUsage::Static)
						Usage = GL_STATIC_DRAW;
					else
						Usage = GL_DYNAMIC_DRAW;


					if (!desc.UsePreciseSize)
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.Size * sizeof(unsigned int), desc.Data, Usage);
					else
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.Size, desc.Data, Usage);


					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				void GLIndexBuffer::Delete(GLIndexBuffer * ibuffer)
				{
					if (ibuffer->buffer != 0)
					{
						glDeleteBuffers(1, &ibuffer->buffer);
					}
					ibuffer->buffer = 0;
				}

				void GLIndexBuffer::Bind()
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
				}
				void GLIndexBuffer::Update(const void * data, unsigned int size)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer);
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
				void * GLIndexBuffer::Map()
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
					return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
				}
				void GLIndexBuffer::Unmap()
				{
					glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}
		}
	}
}

#endif