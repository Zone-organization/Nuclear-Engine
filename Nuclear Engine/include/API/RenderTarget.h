#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLRenderTarget.h>
#include <API\DirectX\DX11RenderTarget.h>

namespace NuclearEngine
{
	namespace API
	{
		class Texture;
		class NEAPI RenderTarget
		{
		public:
			RenderTarget();
			~RenderTarget();

			static void	Create(RenderTarget* shader);
			static void	Delete(RenderTarget* shader);

			void AttachDepthStencilBuffer(const Math::Vector2ui& size);

			void AttachTexture(Texture* texture);

			void Bind();
			static void Bind_Default();
		private:
			OpenGL::GLRenderTarget GLObject;
			DirectX::DX11RenderTarget DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLRenderTarget.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLRenderTarget RenderTarget;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11RenderTarget.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11RenderTarget RenderTarget;
	}
}
#endif
#endif