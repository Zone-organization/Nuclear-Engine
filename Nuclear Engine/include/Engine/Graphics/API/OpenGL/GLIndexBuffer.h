#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct IndexBufferDesc;

			namespace OpenGL
			{
				class NEAPI GLIndexBuffer
				{
				public:
					GLIndexBuffer();
					~GLIndexBuffer();

					static void Create(GLIndexBuffer* buffer, const IndexBufferDesc& desc);
					static void Delete(GLIndexBuffer* ibuffer);

					void Bind();			
					void Update(const void* data, unsigned int size);

				private:
					GLuint buffer;
				};
			}
		}
	}
}
#endif