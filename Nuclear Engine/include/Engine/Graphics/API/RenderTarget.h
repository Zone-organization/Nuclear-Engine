#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLRenderTarget.h>
#include <Engine/Graphics/API/DirectX\DX11RenderTarget.h>

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

				static void	Create(RenderTarget* aRenderT);
				static void	Delete(RenderTarget* aRenderT);

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