#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
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
				static void Delete(GLVertexBuffer* buffer);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout * layout, GLShader *shader);

				void Bind();

			private:
				GLuint VAO;
				GLuint VBO;
			};
		}
	}
}
#endif