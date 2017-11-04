#include "NuclearRendererOGL3\GLIndexBuffer.h"
#include <iostream>
namespace NuclearRenderer {

	GLIndexBuffer::GLIndexBuffer() : buffer(0)
	{
	}
	void GLIndexBuffer::Delete()
	{
		glDeleteBuffers(1, &buffer);
	}

	void GLIndexBuffer::Create(const void * indices, unsigned int count)
	{
		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	}
	void GLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
}
