#pragma once
#include <NE_Common.h>
#include <API\Color.h>

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
			
		};
	}
}