#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/API/RenderAPI.h>

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
					static void DrawIndexed(unsigned int vertexCount, unsigned int StartIndexLocation, int BaseVertexLocation);
					static void SetIndicesType(const IndicesFormat& type);

					static void SetViewPort(int x, int y, int width, int height);
					static void SetScissors(int x, int y, int width, int height);

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

					//OpenGL only!
					static void Query_VAO_EBO_State();
				};
			}
		}
	}
}

#endif