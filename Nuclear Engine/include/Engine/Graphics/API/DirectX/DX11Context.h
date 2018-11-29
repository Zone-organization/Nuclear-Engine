#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/Color.h>
#include <SFML/Window.hpp>
#include <Engine/Graphics/API/RenderAPI.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				class DX11RenderTarget;
				class NEAPI DX11Context
				{
				public:

					static void SetPrimitiveType(int primitivetype);

					static void Clear(Graphics::Color color, Uint32 flags, float depth = 1.0f, unsigned int stencil = 0);

					static void Draw(unsigned int count);
					static void DrawIndexed(unsigned int vertexCount, unsigned int StartIndexLocation, int BaseVertexLocation);

					static void SetViewPort(int x, int y, int width, int height);
					static void SetScissors(int x, int y, int width, int height);

					//DX11 Specific
					static ID3D11Device* GetDevice();
					static ID3D11DeviceContext* GetContext();

					static void Bind_RenderTarget(DX11RenderTarget* rt);
					static void Bind_Default_Rasterizer_State();
					static void Bind_Default_RenderTarget();

					static bool Initialize(sf::Window* window);
					static void Shutdown();
					static void SwapBuffers();

					//UINT                        ScissorRectsCount, ViewportsCount();
				    //D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]();
			    	//D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]();
					static RasterizerState      GetRasterizerState();
					static BlendState           GetBlendState();
					static DepthStencilState    GetDepthStencilState();
					static Texture			    GetPSTexture();
					static Sampler			    GetPSSampler();
					static PixelShader          GetPixelShader();
					static VertexShader         GetVertexShader();
					static IndexBuffer          GetIndexBuffer();
					static VertexBuffer         GetVertexBuffer();
					static ConstantBuffer       GetConstantBuffer();
				};
			}
		}
	}
}

#endif