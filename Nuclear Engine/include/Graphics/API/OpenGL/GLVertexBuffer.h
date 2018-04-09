#pragma once
#include <Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			class InputLayout;
			struct VertexBufferDesc;

			namespace OpenGL
			{
				class GLVertexShader;
				class NEAPI GLVertexBuffer
				{
				public:
					GLVertexBuffer();
					~GLVertexBuffer();

					static void Create(GLVertexBuffer* buffer, const VertexBufferDesc& desc);
					static void Delete(GLVertexBuffer* buffer);

					void Update(const void* data, unsigned int size);

					void SetInputLayout(InputLayout * layout, GLVertexShader *shader);

					void Bind();
					void* Map();
					void Unmap();
				private:
					GLuint VAO;
					GLuint VBO;
				};
			}
		}
	}
}
#endif