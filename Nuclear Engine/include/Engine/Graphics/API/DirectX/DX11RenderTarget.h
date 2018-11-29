#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/Texture_Types.h>
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

					void AttachTexture(DX11Texture* texture, const Texture_Desc& Desc);

					void Bind();
					static void Bind_Default();

					ID3D11Texture2D * mDepthStencilTex;
					ID3D11DepthStencilView* mDepthStencilView;

					std::vector<ID3D11RenderTargetView*> mRTVs;
				};
			}
		}
	}
}
#endif