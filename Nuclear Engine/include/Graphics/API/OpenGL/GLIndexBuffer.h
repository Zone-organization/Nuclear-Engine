#pragma once
#include <Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				class NEAPI GLIndexBuffer
				{
				public:
					GLIndexBuffer();
					~GLIndexBuffer();

					static void Create(GLIndexBuffer* buffer, const void* indices, unsigned int count);
					static void Delete(GLIndexBuffer* ibuffer);

					void Bind();
				private:
					GLuint buffer;
				};
			}
		}
	}
}
#endif