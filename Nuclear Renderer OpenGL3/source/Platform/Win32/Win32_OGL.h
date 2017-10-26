#pragma once
#include "NuclearRendererOGL3\GL_Common.h"
#ifdef _WIN32
#define GLEW_STATIC
#include <GLEW\include\glew.h>
#include <GLEW\include\wglew.h>
#include <gl\gl.h>
#include <gl\glu.h>

namespace NuclearRenderer {
	namespace Internals {

		class NROGL3API Win32_OGL
		{
		public:
			static bool Initialize();

			static void Shutdown();

			static void SwapBuffer();
		};

	}
}
#endif