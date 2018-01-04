#pragma once
#include <API\OpenGL\GLCommon.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <gl\gl.h>
#include <gl\glu.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			namespace Internals {

				class NEAPI Win32_OGL
				{
				public:
					static bool Initialize();

					static void Shutdown();

					static void SwapBuffer();
				};
			}
		}
	}
}
#endif