#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLRenderTarget.h>
#include <Graphics/API/DirectX\DX11RenderTarget.h>

namespace NuclearEngine
{
	namespace Graphics
	{

		namespace API
		{
			class Texture;
			class NEAPI RenderTarget //<OpenGL::GLRenderTarget, DirectX::DX11RenderTarget>
			{
			public:
				RenderTarget();
				~RenderTarget();

				static void	Create(RenderTarget* shader);
				static void	Delete(RenderTarget* shader);

				void AttachDepthStencilBuffer(const Math::Vector2ui& size);

				void AttachTexture(Texture* texture);

				void Bind();

				//TODO: MOVE!! TOO UGLY
				static void Bind_Default();

				BASE_API(RenderTarget)
			};
		}
	}
}