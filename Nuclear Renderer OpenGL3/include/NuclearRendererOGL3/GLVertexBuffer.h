#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
//#include "API\InputLayout.h"
//#include "Core\Renderer\GLRenderer.h"
#include <NuclearRendererBase/NRBVertexBuffer.h>

#define GLEW_STATIC
#include <GLEW\include\glew.h>
#include <GLEW\include\wglew.h>
#include <gl\gl.h>
#include <gl\glu.h>

namespace NuclearRenderer {
	enum class DataType;

	class NROGL3API GLVertexBuffer : public NRBVertexBuffer
	{
	public:
		GLVertexBuffer();
		~GLVertexBuffer();

		void Delete() override;

		void Create(const void* data, unsigned int size, int usage, int access) override;

		void Update(void* data, unsigned int size) override;

		void SetInputLayout(NRBInputLayout * layout, NRBShader *shader) override;

		GLuint GetVertexBuffer() { return VertexBuffer; }

		void Bind() override;
		void Unbind() override;

	private:
		GLenum GetGLDataType(DataType dataType);
		unsigned int GetComponentCount(DataType dataType);

		GLuint VAO;
		GLuint VertexBuffer;
	};

}