#include <Engine/Graphics/API/OpenGL\GLVertexBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/InputLayout.h>
#include <Engine/Graphics/API/Buffer_Types.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLenum GetGLDataType(Format dataType);
				unsigned int GetDataTypeSizeInBytes(Format dataType);

				unsigned int GetComponentCount(Format dataType);

				GLVertexBuffer::GLVertexBuffer()
				{
					VAO = 0;
					VBO = 0;
				}

				GLVertexBuffer::~GLVertexBuffer()
				{
					VAO = 0;
					VBO = 0;
				}

				void GLVertexBuffer::Create(GLVertexBuffer * buffer, const VertexBufferDesc& desc)
				{
					glGenVertexArrays(1, &buffer->VAO);
					glBindVertexArray(buffer->VAO);

					glGenBuffers(1, &buffer->VBO);
					glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
					if (desc.UsageType == Graphics::API::BufferUsage::Static)
					{
						glBufferData(GL_ARRAY_BUFFER, desc.Size, desc.Data, GL_STATIC_DRAW);
					}
					else
					{
						glBufferData(GL_ARRAY_BUFFER, desc.Size, desc.Data, GL_DYNAMIC_DRAW);
					}
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

				}

				void GLVertexBuffer::Delete(GLVertexBuffer * buffer)
				{

					glDeleteBuffers(1, &buffer->VAO);
					glDeleteBuffers(1, &buffer->VBO);

					buffer->VAO = 0;
					buffer->VBO = 0;
				}

				void GLVertexBuffer::Update(const void * data, unsigned int size)
				{
					glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}

				void GLVertexBuffer::SetInputLayout(InputLayout * layout, GLVertexShader * shader)
				{
					unsigned int index = 0;
					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);

					//Calculate Stride & Offset
					unsigned int stride = 0;
					unsigned int offset = 0;

					for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
					{
						stride = stride + GetDataTypeSizeInBytes(layout->GetBufferElement()[i].format);
					}

					for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index,
							GetComponentCount(layout->GetBufferElement()[i].format),
							GetGLDataType(layout->GetBufferElement()[i].format),
							GL_FALSE, stride,
							(GLvoid*)offset);

						index++;

						offset = offset + GetDataTypeSizeInBytes(layout->GetBufferElement()[i].format);
					}

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}

				void GLVertexBuffer::Bind()
				{
					glBindVertexArray(VAO);
				}

				void * GLVertexBuffer::Map()
				{
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				}

				void GLVertexBuffer::Unmap()
				{
					glUnmapBuffer(GL_ARRAY_BUFFER);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}

				GLenum GetGLDataType(Format dataType)
				{

					switch (dataType)
					{
					case Format::R32_FLOAT:
					case Format::R32G32_FLOAT:
					case Format::R32G32B32_FLOAT:
					case Format::R32G32B32A32_FLOAT:
					{		return GL_FLOAT;	}
					case Format::R8G8B8A8_UNORM:
					{	return GL_UNSIGNED_BYTE;	}
					default:
						return -1;
						break;
					}
				}

				unsigned int GetDataTypeSizeInBytes(Format format)
				{
					switch (format)
					{
					case Format::R32_FLOAT:
					{	return 4;	}
					case Format::R32G32_FLOAT:
					{	return 8;	}
					case Format::R32G32B32_FLOAT:
					{	return 12;	}
					case Format::R32G32B32A32_FLOAT:
					{	return 16;	}
					case Format::R8G8B8A8_UNORM:
					{	return 4;	}
					default:
						return -1;
					}
				}

				unsigned int GetComponentCount(Format dataType)
				{
					switch (dataType)
					{
					case Format::R32_FLOAT:
					{ return 1;	}
					case Format::R32G32_FLOAT:
					{ return 2; }
					case Format::R32G32B32_FLOAT:
					{ return 3; }
					case Format::R32G32B32A32_FLOAT:
					{ return 4; }
					case Format::R8G8B8A8_UNORM:
					{ return 4;	}
					default:
						return -1;
						break;
					}
				}

			}
		}
	}
}
#endif