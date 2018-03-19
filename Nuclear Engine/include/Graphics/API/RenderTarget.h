#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics/API/OpenGL\GLRenderTarget.h>
#include <Graphics/API/DirectX\DX11RenderTarget.h>

namespace NuclearEngine
{
	namespace Graphics
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
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLRenderTarget.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef OpenGL::GLRenderTarget RenderTarget;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11RenderTarget.h>
namespace NuclearEngine
{
	namespace Graphics
	{

		namespace API
		{
			typedef DirectX::DX11RenderTarget RenderTarget;
		}
	}
}
#endif
#endif