#pragma once
#include <NE_Common.h>
#include <Graphics/Color.h>
#include <Core\Application.h>

#define	ClearColorBuffer 1
#define	ClearDepthBuffer 2
#define	ClearStencilBuffer 4

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

		namespace API {
			namespace ContextDesc
			{
				//IF 0 then Ansiotrpopic isnot supported
				static float MaxAnisotropicLevel = 0.0f;
			}

			class NEAPI Context
			{
				friend class Core::Application;
			public:
				static Core::RenderAPI GetRenderAPI();

				static void SetPrimitiveType(PrimitiveType primitivetype);

				static void Clear(Graphics::Color color, uint flags, float depth = 1.0f, unsigned int stencil = 0);

				//Render the frame to the window
				static void PresentFrame();

				static void Draw(unsigned int count);
				static void DrawIndexed(unsigned int vertexCount);

				static void SetViewPort(int x, int y, int width, int height);

				static bool isOpenGL3RenderAPI();
				static bool isDirectX11RenderAPI();

			private:
				static void SetRenderAPI(const Core::RenderAPI & renderer);
			};
		}
	}
}