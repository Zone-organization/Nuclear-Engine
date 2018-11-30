#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>

#ifdef NE_COMPILE_CORE_OPENGL
static int ubosBindingindex = 0;


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
					this->mCBuffer = 0;
					this->mName = "NewCB";
					this->mBindingIndex = 0;
				}
				GLConstantBuffer::~GLConstantBuffer()
				{
					this->mCBuffer = 0;
					this->mName = "OldCB";
					this->mBindingIndex = 0;
				}
				void GLConstantBuffer::Create(GLConstantBuffer* result, const char * Nameinshader, unsigned int size)
				{
					int remainder = size % 16;
					if (remainder != 0)
					{
						Log.Warning("[GLConstantBuffer] The size of mCBuffer isn't a multiple of 16 which can cause many unexpected problems! \n");
					}

					result->mName = Nameinshader;
					result->mBindingIndex = ubosBindingindex;

					glGenBuffers(1, &result->mCBuffer);
					glBindBuffer(GL_UNIFORM_BUFFER, result->mCBuffer);
					glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

					glBindBufferRange(GL_UNIFORM_BUFFER, result->mBindingIndex, result->mCBuffer, 0, size);

					//Increment the binding index
					ubosBindingindex++;
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				void GLConstantBuffer::Delete(GLConstantBuffer * cmCBuffer)
				{
					glDeleteBuffers(1, &cmCBuffer->mCBuffer);					
					cmCBuffer->mCBuffer = 0;
				}

				void GLConstantBuffer::Update(const void* data, unsigned int size)
				{
					glBindBuffer(GL_UNIFORM_BUFFER, mCBuffer);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				void GLConstantBuffer::VSBind(Uint8 slot)
				{
				}

				void GLConstantBuffer::PSBind(Uint8 slot)
				{
				}
				void GLConstantBuffer::GSBind(Uint8 slot)
				{
				}
			}
		}
	}
}

#endif