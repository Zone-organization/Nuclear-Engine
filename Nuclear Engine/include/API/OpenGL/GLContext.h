#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
namespace NuclearEngine
{
	namespace API 
	{
		namespace OpenGL 
		{
			
			class NEAPI GLContext
			{
			public:
				bool Initialize();
				void SetPrimitiveType(int primitivetype);
				void ClearColor(float Red, float Green, float Blue, float Alpha);				
				void ClearDepthBuffer();
				void ClearStencilBuffer();

				void EnableDepthBuffer(bool state);

				void SwapBuffers();
				void Shutdown();

				void Draw(unsigned int count);
				void DrawIndexed(unsigned int vertexCount);

				void SetViewPort(int x, int y, int width, int height);
			};
		}
	}
}

#endif