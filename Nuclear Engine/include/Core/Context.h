#pragma once
#include <NE_Common.h>
#include <API\Color.h>

namespace NuclearPlatform
{
	class Window;
}
namespace NuclearRenderer
{
	class NRBContext;
	class NRBVertexBuffer;
	class NRBIndexBuffer;
	class NRBUniformBuffer;
	class NRBTexture2D;
	class NRBTextureCube;
	class NRBPipelineState;
	class NRBShader;
}
namespace NuclearEngine
{
	enum PrimitiveType 
	{ 
		PointList = 0,
		LineList = 1,
		LineStrip = 2,
		TriangleList = 3,
		TriangeStrip = 4
	};

	namespace Core {
		namespace ContextDesc
		{
			static float MaxAnisotropicLevel;
			static bool SupportAnisotropic = false;
		}
		enum class NEAPI RenderAPI {
			OpenGL3,
			DirectX11
		};
		class NEAPI Context
		{
		public:
			static bool Initialize(NuclearRenderer::NRBContext *context );
			static bool Initialize(const RenderAPI& renderer);

			static RenderAPI GetRenderAPI();

			static void SetPrimitiveType(PrimitiveType primitivetype);
			static void ClearColor(API::Color color);
			static void ClearDepthBuffer();
			static void ClearStencilBuffer();

			static void Shutdown();

			static void Begin();
			static void End();

			static void EnableDepthBuffer(bool state);

			static void Draw(unsigned int count);
			static void DrawIndexed(unsigned int vertexCount);

			static void SetViewPort(int x, int y, int width, int height);
			
			static NuclearRenderer::NRBVertexBuffer* ConstructVertexBuffer(NuclearRenderer::NRBVertexBuffer* param);
			static NuclearRenderer::NRBIndexBuffer* ConstructIndexBuffer(NuclearRenderer::NRBIndexBuffer* param);
			static NuclearRenderer::NRBUniformBuffer* ConstructUniformBuffer(NuclearRenderer::NRBUniformBuffer* param);
			static NuclearRenderer::NRBTexture2D* ConstructTexture2D(NuclearRenderer::NRBTexture2D* param);
			static NuclearRenderer::NRBPipelineState* ConstructPipelineState(NuclearRenderer::NRBPipelineState* param);
			static NuclearRenderer::NRBTextureCube* ConstructTextureCube(NuclearRenderer::NRBTextureCube* param);
			static NuclearRenderer::NRBShader* ConstructShader(NuclearRenderer::NRBShader* param);
		};
	}
}