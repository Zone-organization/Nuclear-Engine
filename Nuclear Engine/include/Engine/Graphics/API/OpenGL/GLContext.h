#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/Color.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{

				class NEAPI GLContext
				{
				public:
					static void SetPrimitiveType(int primitivetype);

					static void Clear(Graphics::Color color, Uint32 flags, float depth = 1.0f, unsigned int stencil = 0);

					static void Draw(unsigned int count);
					static void DrawIndexed(unsigned int vertexCount);

					static void SetViewPort(int x, int y, int width, int height);

					//OpenGL only!
					static void Query_VAO_EBO_State();
				};
			}
		}
	}
}

#endif