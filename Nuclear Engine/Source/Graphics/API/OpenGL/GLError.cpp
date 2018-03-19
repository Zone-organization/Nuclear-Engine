#pragma once
#include <Graphics/API/OpenGL\GLError.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLenum glCheckError(const char *file, int line)
				{
					GLenum errorCode;
					while ((errorCode = glGetError()) != GL_NO_ERROR)
					{
						std::string error;
						switch (errorCode)
						{
						case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
						case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
						case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
						case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
						case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
						case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
						case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
						}
						Log.Error("[OpenGL Error] File: " + std::string(file) + " Line: " + std::to_string(line) + " Error: " + error + "\n");
					}
					return errorCode;
				}
			}
		}
	}
}
#endif
