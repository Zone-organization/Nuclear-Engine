#include <NuclearRendererOGL3\GLConstantBuffer.h>
static int ubosBindingindex = 0;

namespace NuclearRenderer
{
	GLConstantBuffer::GLConstantBuffer()
	{
	}	
	void GLConstantBuffer::Create(const char * Nameinshader, unsigned int size)
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

	void GLConstantBuffer::Update(const void* data, unsigned int size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GLConstantBuffer::Delete()
	{
		glDeleteBuffers(1, &buffer);
	}

	unsigned int GLConstantBuffer::GetBindingIndex()
	{
		return m_Bindingindex;
	}

	const char * GLConstantBuffer::GetName()
	{
		return name;
	}
	ID3D11Buffer * const * GLConstantBuffer::GetDXBuffer()
	{
		return nullptr;
	}
}