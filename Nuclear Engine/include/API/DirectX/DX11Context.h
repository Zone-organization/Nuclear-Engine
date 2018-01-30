#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\Color.h>
#include <API\API_Types.h>
#include <Core\Application.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class NEAPI DX11Context
			{
				friend Core::Application;
			public:

				static void SetPrimitiveType(int primitivetype);

				static void Clear(API::Color color, uint flags, float depth = 1.0f, unsigned int stencil = 0);

				static void EnableDepthBuffer(bool state);

				static void Draw(unsigned int count);
				static void DrawIndexed(unsigned int vertexCount);

				static void SetViewPort(int x, int y, int width, int height);

				static ID3D11Device* GetDevice();
				static ID3D11DeviceContext* GetContext();
			private:
				static bool Initialize(GLFWwindow* window);
				static void Shutdown();
				static void SwapBuffers();
			};
		}
	}
}

#endif