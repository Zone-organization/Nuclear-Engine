#pragma once
#include <NE_Common.h>
#include <API\Texture2D.h>

namespace NuclearRenderer
{
	class NRBRenderTarget;
}
namespace NuclearEngine
{
	namespace API 
	{
		class NEAPI RenderTarget
		{
		public:
			RenderTarget();

			~RenderTarget();

			void AttachDepthBuffer(Texture2D* texture);

			void AttachStencilBuffer(Texture2D* texture);

			void AttachDepthStencilBuffer(Texture2D* texture);

			void AttachTexture(Texture2D* texture);

			void Bind();

			void Unbind();

		private:
			NuclearRenderer::NRBRenderTarget* Buf;
		};
	}
}