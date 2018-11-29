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
				GLboolean IsFormatNormalized(Format dataType);

				unsigned int GetDataTypeSizeInBytes(Format dataType);

				unsigned int GetComponentCount(Format dataType);

				GLVertexBuffer::GLVertexBuffer()
				{
					mVAO = 0;
					mVBO = 0;
				}

				GLVertexBuffer::~GLVertexBuffer()
				{
					mVAO = 0;
					mVBO = 0;
				}

				void GLVertexBuffer::Create(GLVertexBuffer * buffer, const VertexBufferDesc& desc)
				{
					glGenVertexArrays(1, &buffer->mVAO);
					glBindVertexArray(buffer->mVAO);

					glGenBuffers(1, &buffer->mVBO);
					glBindBuffer(GL_ARRAY_BUFFER, buffer->mVBO);
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

					glDeleteBuffers(1, &buffer->mVAO);
					glDeleteBuffers(1, &buffer->mVBO);

					buffer->mVAO = 0;
					buffer->mVBO = 0;
				}

				void GLVertexBuffer::Update(const void * data, unsigned int size)
				{
					glBindBuffer(GL_ARRAY_BUFFER, this->mVBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}

				void GLVertexBuffer::SetInputLayout(InputLayout * layout, GLVertexShader * shader)
				{
					unsigned int index = 0;
					glBindVertexArray(mVAO);
					glBindBuffer(GL_ARRAY_BUFFER, mVBO);

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
							IsFormatNormalized(layout->GetBufferElement()[i].format), stride,
							(GLvoid*)offset);

						index++;

						offset = offset + GetDataTypeSizeInBytes(layout->GetBufferElement()[i].format);
					}

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}

				void GLVertexBuffer::Bind()
				{
					glBindVertexArray(mVAO);
				}

				void * GLVertexBuffer::Map()
				{
					glBindBuffer(GL_ARRAY_BUFFER, mVBO);
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

				GLboolean IsFormatNormalized(Format dataType)
				{
					switch (dataType)
					{
					case Format::R32_FLOAT:
					case Format::R32G32_FLOAT:
					case Format::R32G32B32_FLOAT:
					case Format::R32G32B32A32_FLOAT:
					{		return GL_FALSE;		}
					case Format::R8_UNORM:
					case Format::R8G8_UNORM:
					case Format::R8G8B8_UNORM:
					case Format::R8G8B8A8_UNORM:
					{	    return GL_TRUE;			}
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