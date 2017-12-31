#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
#include <API\OpenGL\GLShader.h>
#include <API\InputLayout.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			class NEAPI GLVertexBuffer 
			{
			public:
				GLVertexBuffer();
				~GLVertexBuffer();

				static void Create(GLVertexBuffer* buffer, VertexBufferDesc* desc);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout * layout, GLShader *shader);

				void Bind();
				void Unbind();

			private:
				GLuint VAO;
				GLuint VBO;
			};
		}
	}
}
#endif