#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/Color.h>
#include <Core\ApplicationDesc.h>

#define	ClearColorBuffer 0b00000001
#define	ClearDepthBuffer 0b00000010
#define	ClearStencilBuffer 0b00000100

namespace NuclearEngine
{
	namespace Graphics
	{

		enum PrimitiveType
		{
			PointList = 0,
			LineList = 1,
			LineStrip = 2,
			TriangleList = 3,
			TriangeStrip = 4
		};

		namespace API
		{
			namespace ContextDesc
			{
				//If 0 then Ansiotropic isn't supported.
				static float MaxAnisotropicLevel = 0.0f;
			}

			enum class IndicesFormat;
			class RasterizerState;
			class BlendState;
			class DepthStencilState;
			class Texture;
			class Sampler;
			class PixelShader;
			class VertexShader;
			class IndexBuffer;
			class VertexBuffer;
			class ConstantBuffer;			
			struct SavedState;

			class NEAPI Context
			{
			public:
				//get active render api
				static Core::RenderAPI GetRenderAPI();

				//Set primitive topology
				static void SetPrimitiveType(PrimitiveType primitivetype);

				//Clears "Color, Depth and Stencil" Buffers
				static void Clear(Graphics::Color color, Uint32 flags, float depth = 1.0f, unsigned int stencil = 0);

				//Render the frame to the window
				static void PresentFrame();

				//Drawing Function
				static void Draw(unsigned int count);
				static void DrawIndexed(unsigned int VerticesCount, unsigned int StartIndexLocation = 0, int BaseVertexLocation = 0);

				static void SetIndicesType(const IndicesFormat& type);
				static void SetViewPort(int x, int y, int width, int height);
				static void SetScissors(int x, int y, int width, int height);

				static bool isOpenGL3RenderAPI();
				static bool isDirectX11RenderAPI();

				static void SetRenderAPI(const Core::RenderAPI & renderer);
				
				static SavedState SaveState();
				static void LoadState(SavedState& state);

				static RasterizerState		GetRasterizerState();
				static BlendState			GetBlendState();
				static DepthStencilState	GetDepthStencilState();
				static Texture				GetPSTexture();
				static Sampler				GetPSSampler();
				static PixelShader			GetPixelShader();
				static VertexShader			GetVertexShader();
				static IndexBuffer			GetIndexBuffer();
				static VertexBuffer			GetVertexBuffer();
				static ConstantBuffer		GetConstantBuffer();
				static IndicesFormat		GetIndicesType();

			};
		}
	}
}