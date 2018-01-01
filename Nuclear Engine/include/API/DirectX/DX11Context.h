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
				bool Initialize();
				void Shutdown();

				void SetPrimitiveType(int primitivetype);

				void Clear(API::Color color, ClearFlags flags, float depth = 1.0f, float stencil = 0.0f);

				void EnableDepthBuffer(bool state);

				void SwapBuffers();

				void Draw(unsigned int count);
				void DrawIndexed(unsigned int vertexCount);

				void SetViewPort(int x, int y, int width, int height);

				static ID3D11Device1* GetDevice();
				static ID3D11DeviceContext1* GetContext();
			};
		}
	}
}

#endif