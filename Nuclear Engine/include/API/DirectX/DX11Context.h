#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\Color.h>
#include <API\API_Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class NEAPI DX11Context
			{
			public:
				static bool Initialize();
				static void Shutdown();

				static void SetPrimitiveType(int primitivetype);

				static void Clear(API::Color color, uint flags, float depth = 1.0f, float stencil = 0.0f);

				static void EnableDepthBuffer(bool state);

				static void SwapBuffers();

				static void Draw(unsigned int count);
				static void DrawIndexed(unsigned int vertexCount);

				static void SetViewPort(int x, int y, int width, int height);

				static ID3D11Device* GetDevice();
				static ID3D11DeviceContext* GetContext();
			};
		}
	}
}

#endif