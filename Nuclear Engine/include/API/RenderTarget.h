#pragma once
#include <NE_Common.h>
#include <API\Texture.h>

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

			void AttachDepthBuffer(Texture* texture);

			void AttachStencilBuffer(Texture* texture);

			void AttachDepthStencilBuffer(Texture* texture);

			void AttachTexture(Texture* texture);

			void Bind();

			void Unbind();

		private:
			NuclearRenderer::NRBRenderTarget* Buf;
		};
	}
}