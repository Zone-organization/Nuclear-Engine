#pragma once
#include <API\OpenGL\GLCommon.h>
#include <API\OpenGL\GLShader.h>
#include <API\API_Types.h>
#include <API\InputLayout.h>

#ifdef NE_COMPILE_OPENGL3_3
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

				static GLVertexBuffer Create(const void* data, unsigned int size, BufferGPUUsage usage, BufferCPUAccess access);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout * layout, GLShader *shader);

				GLuint GetVertexBuffer();

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