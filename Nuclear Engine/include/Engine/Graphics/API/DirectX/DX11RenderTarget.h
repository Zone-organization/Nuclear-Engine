#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Base\Math\Math.h>
#include <vector>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				class DX11Texture;
				class DX11Context;
				class NEAPI DX11RenderTarget
				{
					friend class DX11Context;
				public:
					DX11RenderTarget();
					~DX11RenderTarget();

					static void Create(DX11RenderTarget* result);
					static void Delete(DX11RenderTarget* result);

					void AttachDepthStencilBuffer(const Math::Vector2ui& size);

					void AttachTexture(DX11Texture* texture);

					void Bind();
					static void Bind_Default();
				private:
					ID3D11Texture2D * depthstenciltex;
					ID3D11DepthStencilView* depthstencilview;

					std::vector<ID3D11RenderTargetView*> rendertargetviews;
				};
			}
		}
	}
}
#endif