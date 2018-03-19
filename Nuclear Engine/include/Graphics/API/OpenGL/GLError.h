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
				GLenum glCheckError(const char *file, int line);

#ifdef _DEBUG
#define GLCheckError() glCheckError(__FILE__, __LINE__) 
#else
#define GLCheckError()
#endif

#ifdef _DEBUG
#define GLCall(stmt) \
				do { stmt; \
					glCheckError(__FILE__, __LINE__);\
				} while (false)
#else
#define GLCall(stmt) stmt
#endif
			}
		}
	}
}
#endif
