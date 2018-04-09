#include <Graphics/API/OpenGL\GLVertexBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLError.h>
#include <Graphics/API/InputLayout.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLenum GetGLDataType(DataType dataType);
				unsigned int GetDataTypeSizeInBytes(DataType dataType);

				unsigned int GetComponentCount(DataType dataType);

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
					GLCall(glGenVertexArrays(1, &buffer->VAO));
					GLCall(glBindVertexArray(buffer->VAO));

					GLCall(glGenBuffers(1, &buffer->VBO));
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO));
					if (desc.usage == Graphics::API::BufferUsage::Static)
					{
						GLCall(glBufferData(GL_ARRAY_BUFFER, desc.size, desc.data, GL_STATIC_DRAW));
					}
					else
					{
						GLCall(glBufferData(GL_ARRAY_BUFFER, desc.size, desc.data, GL_DYNAMIC_DRAW));
					}
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
					GLCall(glBindVertexArray(0));

				}

				void GLVertexBuffer::Delete(GLVertexBuffer * buffer)
				{

					GLCall(glDeleteBuffers(1, &buffer->VAO));
					GLCall(glDeleteBuffers(1, &buffer->VBO));

					buffer->VAO = 0;
					buffer->VBO = 0;
				}

				void GLVertexBuffer::Update(const void * data, unsigned int size)
				{
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));
					GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				}

				void GLVertexBuffer::SetInputLayout(InputLayout * layout, GLVertexShader * shader)
				{
					unsigned int index = 0;
					GLCall(glBindVertexArray(VAO));
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

					//Calculate Stride & Offset
					unsigned int stride = 0;
					unsigned int offset = 0;

					for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
					{
						stride = stride + GetDataTypeSizeInBytes(layout->GetBufferElement()[i].dataType);
					}

					for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
					{
						GLCall(glEnableVertexAttribArray(index));
						GLCall(glVertexAttribPointer(index,
							GetComponentCount(layout->GetBufferElement()[i].dataType),
							GetGLDataType(layout->GetBufferElement()[i].dataType),
							GL_FALSE, stride,
							(GLvoid*)offset));

						index++;

						offset = offset + GetDataTypeSizeInBytes(layout->GetBufferElement()[i].dataType);
					}

					GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
					GLCall(glBindVertexArray(0));
				}

				void GLVertexBuffer::Bind()
				{
					GLCall(glBindVertexArray(VAO));
				}

				void * GLVertexBuffer::Map()
				{
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
					GLCall(return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
				}

				void GLVertexBuffer::Unmap()
				{
					GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
					GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				}

				GLenum GetGLDataType(DataType dataType)
				{

					switch (dataType)
					{
					case DataType::Float:
					{
						return GL_FLOAT;
						break;
					}
					case DataType::Float2:
					{
						return GL_FLOAT;
						break;
					}
					case DataType::Float3:
					{
						return GL_FLOAT;
						break;
					}
					case DataType::Float4:
					{
						return GL_FLOAT;
						break;
					}
					default:
						return -1;
						break;
					}
				}

				unsigned int GetDataTypeSizeInBytes(DataType dataType)
				{
					switch (dataType)
					{
					case DataType::Float:
					{
						return 4;
						break;
					}
					case DataType::Float2:
					{
						return 8;
						break;
					}
					case DataType::Float3:
					{
						return 12;
						break;
					}
					case DataType::Float4:
					{
						return 16;
						break;
					}
					default:
						return -1;
						break;
					}
				}

				unsigned int GetComponentCount(DataType dataType)
				{
					switch (dataType)
					{
					case DataType::Float:
					{
						return 1;
						break;
					}
					case DataType::Float2:
					{
						return 2;
						break;
					}
					case DataType::Float3:
					{
						return 3;
						break;
					}
					case DataType::Float4:
					{
						return 4;
						break;
					}
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