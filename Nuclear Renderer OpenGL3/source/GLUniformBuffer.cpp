#include <NuclearRendererOGL3\GLUniformBuffer.h>

namespace NuclearRenderer
{
	GLUniformBuffer::GLUniformBuffer()
	{
	}	
	void GLUniformBuffer::Create(const char * Nameinshader, unsigned int size, unsigned int Bindingindex)
	{
		this->name = Nameinshader;
		this->m_Bindingindex = Bindingindex;

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, m_Bindingindex, buffer, 0, size);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GLUniformBuffer::Update(void * data, unsigned int offset, unsigned int size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
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
}