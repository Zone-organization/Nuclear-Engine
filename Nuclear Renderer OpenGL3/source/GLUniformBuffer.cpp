#include <NuclearRendererOGL3\GLUniformBuffer.h>
static int ubosBindingindex = 0;

namespace NuclearRenderer
{
	GLUniformBuffer::GLUniformBuffer()
	{
	}	
	void GLUniformBuffer::Create(const char * Nameinshader, unsigned int size)
	{
		this->name = Nameinshader;
		this->m_Bindingindex = ubosBindingindex;

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, this->m_Bindingindex, buffer, 0, size);

		//Increment the binding index
		ubosBindingindex++;
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	void GLUniformBuffer::Update(const void* data, unsigned int size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GLUniformBuffer::Delete()
	{
		glDeleteBuffers(1, &buffer);
	}

	unsigned int GLUniformBuffer::GetBindingIndex()
	{
		return m_Bindingindex;
	}

	const char * GLUniformBuffer::GetName()
	{
		return name;
	}
	ID3D11Buffer * const * GLUniformBuffer::GetDXBuffer()
	{
		return nullptr;
	}
}