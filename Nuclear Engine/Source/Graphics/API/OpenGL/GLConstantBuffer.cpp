#include <Graphics/API/OpenGL\GLConstantBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
static int ubosBindingindex = 0;
#include <Graphics/API/OpenGL\GLError.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLConstantBuffer::GLConstantBuffer()
				{
					this->buffer = 0;
					this->name = "unset";
					this->BindingIndex = 0;
				}
				GLConstantBuffer::~GLConstantBuffer()
				{
					this->buffer = 0;
					this->name = "deleted";
					this->BindingIndex = 0;
				}
				void GLConstantBuffer::Create(GLConstantBuffer* result, const char * Nameinshader, unsigned int size)
				{
					int remainder = size % 16;
					if (remainder != 0)
					{
						Log.Warning("[GLConstantBuffer] The size of buffer isn't a multiple of 16 which can cause many unexpected problems! \n");
					}

					result->name = Nameinshader;
					result->BindingIndex = ubosBindingindex;

					GLCall(glGenBuffers(1, &result->buffer));
					GLCall(glBindBuffer(GL_UNIFORM_BUFFER, result->buffer));
					GLCall(glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW));

					GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, result->BindingIndex, result->buffer, 0, size));

					//Increment the binding index
					ubosBindingindex++;
					GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
				}

				void GLConstantBuffer::Delete(GLConstantBuffer * cbuffer)
				{
					if (cbuffer->buffer != 0)
					{
						GLCall(glDeleteBuffers(1, &cbuffer->buffer));
					}

					cbuffer->buffer = 0;
				}

				void GLConstantBuffer::Update(const void* data, unsigned int size)
				{
					GLCall(glBindBuffer(GL_UNIFORM_BUFFER, buffer));
					GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data));
					GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
				}

				unsigned int GLConstantBuffer::GetBindingIndex()
				{
					return BindingIndex;
				}

				const char * GLConstantBuffer::GetName()
				{
					return name;
				}

			}
		}
	}
}

#endif