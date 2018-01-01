#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
#include <API\Color.h>
#include <API\API_Types.h>

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

				void Clear(API::Color color, ClearFlags flags, float depth = 1.0f, float stencil = 0.0f);

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