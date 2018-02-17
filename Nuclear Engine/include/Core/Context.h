#pragma once
#include <NE_Common.h>
#include <API\Color.h>
#include <Core\Application.h>

#define	ClearColorBuffer 1
#define	ClearDepthBuffer 2
#define	ClearStencilBuffer 4

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
			//IF 0 then Ansiotrpopic isnot supported
			static float MaxAnisotropicLevel = 0.0f;
		}
	
		class NEAPI Context
		{
			friend class Application;
		public:
			static RenderAPI GetRenderAPI();

			static void SetPrimitiveType(PrimitiveType primitivetype);

			static void Clear(API::Color color, uint flags, float depth = 1.0f, unsigned int stencil = 0);

			static void Begin();
			static void End();

			static void Draw(unsigned int count);
			static void DrawIndexed(unsigned int vertexCount);

			static void SetViewPort(int x, int y, int width, int height);
			
		private:
			static void SetRenderAPI(const RenderAPI & renderer);
		};
	}
}