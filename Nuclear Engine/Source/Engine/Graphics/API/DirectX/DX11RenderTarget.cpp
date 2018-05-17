#include <Engine/Graphics/API/DirectX\DX11RenderTarget.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Texture.h>
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/DirectX\DX11Types.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				DX11RenderTarget::DX11RenderTarget()
				{
					depthstenciltex = NULL;
					depthstencilview = NULL;
				}
				DX11RenderTarget::~DX11RenderTarget()
				{
					depthstenciltex = NULL;
					depthstencilview = NULL;
				}
				void DX11RenderTarget::Create(DX11RenderTarget * result)
				{

				}
				void DX11RenderTarget::Delete(DX11RenderTarget * result)
				{
					for (size_t i = 0; i < result->rendertargetviews.size(); i++)
					{
						result->rendertargetviews.at(i)->Release();
					}
					result->rendertargetviews.clear();
				}

				void DX11RenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
				{
					D3D11_TEXTURE2D_DESC texDesc;
					{
						texDesc.Width = size.x;
						texDesc.Height = size.y;
						texDesc.MipLevels = 1;
						texDesc.ArraySize = 1;
						texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
						texDesc.SampleDesc.Count = 1;
						texDesc.SampleDesc.Quality = 0;
						texDesc.Usage = D3D11_USAGE_DEFAULT;
						texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
						texDesc.CPUAccessFlags = 0;
						texDesc.MiscFlags = 0;
					}
					if (FAILED(Graphics::API::DirectX::DX11Context::GetDevice()->CreateTexture2D(&texDesc, nullptr, &depthstenciltex)))
					{
						Log.Error("[DX11RenderTarget] Failed to create DepthStencilTexture for render-target!\n");
						//Return or the next call will fail too
						return;
					}
					if (FAILED(Graphics::API::DirectX::DX11Context::GetDevice()->CreateDepthStencilView(depthstenciltex, nullptr, &depthstencilview)))
					{
						Log.Error("[DX11RenderTarget] Failed to create DepthStencilView for render-target!\n");
					}
				}
				void DX11RenderTarget::AttachTexture(DX11Texture * texture)
				{
					D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
					ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
					renderTargetViewDesc.Format = DXTypeMap(texture->GetTextureDesc().Format);

					switch (texture->GetTextureDesc().Type)
					{
					case TextureType::Texture2D:
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
						renderTargetViewDesc.Texture2D.MipSlice = 0;
					default:
						Exceptions::NotImplementedException();
					}
					ID3D11RenderTargetView* rtv;

					if (FAILED(Graphics::API::DirectX::DX11Context::GetDevice()->CreateRenderTargetView(texture->tex2D, &renderTargetViewDesc, &rtv)))
					{
						Log.Error("[DX11RenderTarget] Failed to create render-target-view (RTV)!\n");
						return;
					}

					rendertargetviews.push_back(rtv);
				}
				void DX11RenderTarget::Bind()
				{
					Graphics::API::DirectX::DX11Context::Bind_RenderTarget(this);
				}

				void DX11RenderTarget::Bind_Default()
				{
					Graphics::API::DirectX::DX11Context::Bind_Default_RenderTarget();
				}

			}
		}
	}
}
#endif