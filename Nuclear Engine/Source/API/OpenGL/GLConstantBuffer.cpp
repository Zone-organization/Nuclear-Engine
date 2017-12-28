#include <API\OpenGL\GLConstantBuffer.h>

#ifdef NE_COMPILE_OPENGL3_3
static int ubosBindingindex = 0;

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			GLConstantBuffer::GLConstantBuffer()
			{
				this->buffer = 0;
			}
			GLConstantBuffer::~GLConstantBuffer()
			{
				if (this->buffer != 0)
				{
					glDeleteBuffers(1, &buffer);
				}

				this->buffer = 0;
			}
			GLConstantBuffer GLConstantBuffer::Create(const char * Nameinshader, unsigned int size)
			{
				GLConstantBuffer result;
				result.name = Nameinshader;
				result.BindingIndex = ubosBindingindex;

				glGenBuffers(1, &result.buffer);
				glBindBuffer(GL_UNIFORM_BUFFER, result.buffer);
				glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

				glBindBufferRange(GL_UNIFORM_BUFFER, result.BindingIndex, result.buffer, 0, size);

				//Increment the binding index
				ubosBindingindex++;
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				return result;
			}

			void GLConstantBuffer::Update(const void* data, unsigned int size)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, buffer);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

#endif