#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
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

				static ID3D11Device1* GetDevice();
				static ID3D11DeviceContext1* GetContext();
			};
		}
	}
}

#endif