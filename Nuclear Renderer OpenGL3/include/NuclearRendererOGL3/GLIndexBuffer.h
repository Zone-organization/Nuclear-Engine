#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBIndexBuffer.h>

#define GLEW_STATIC
#include <GLEW\include\glew.h>
#include <GLEW\include\wglew.h>
#include <gl\gl.h>
#include <gl\glu.h>

namespace NuclearRenderer {

	class NROGL3API GLIndexBuffer : public NRBIndexBuffer
	{
	public:
		GLIndexBuffer();

		void Delete() override;

		void Create(void* indices, unsigned int count) override;

		void Bind() override;
		void Unbind() override;

	private:
		GLuint buffer;
	};
}