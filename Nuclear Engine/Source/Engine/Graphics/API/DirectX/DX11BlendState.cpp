#pragma once
#include <Engine/Graphics/API/DirectX\DX11BlendState.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/RenderStates_Types.h>
#include <Engine/Graphics/API/DirectX\DX11Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				DX11BlendState::DX11BlendState()
				{
					mBState = nullptr;
				}
				DX11BlendState::~DX11BlendState()
				{
					mBState = nullptr;
				}
				void DX11BlendState::Create(DX11BlendState * state, const BlendStateDesc & type)
				{
					D3D11_BLEND_DESC BSDesc;
					ZeroMemory(&BSDesc, sizeof(D3D11_BLEND_DESC));
					BSDesc.AlphaToCoverageEnable = type.AlphaToCoverageEnable;
					BSDesc.IndependentBlendEnable = type.IndependentBlendEnable;
					for (unsigned int i = 0; i < 8; i++)
					{
						BSDesc.RenderTarget[i].BlendEnable = type.RenderTarget[i].BlendEnable;
						BSDesc.RenderTarget[i].SrcBlend = (D3D11_BLEND)type.RenderTarget[i].SrcBlend;
						BSDesc.RenderTarget[i].DestBlend = (D3D11_BLEND)type.RenderTarget[i].DestBlend;
						BSDesc.RenderTarget[i].BlendOp = (D3D11_BLEND_OP)type.RenderTarget[i].BlendOp;
						BSDesc.RenderTarget[i].SrcBlendAlpha = (D3D11_BLEND)type.RenderTarget[i].SrcBlendAlpha;
						BSDesc.RenderTarget[i].DestBlendAlpha = (D3D11_BLEND)type.RenderTarget[i].DestBlendAlpha;
						BSDesc.RenderTarget[i].BlendOpAlpha = (D3D11_BLEND_OP)type.RenderTarget[i].BlendOpAlpha;
						BSDesc.RenderTarget[i].RenderTargetWriteMask = type.RenderTarget[i].RenderTargetWriteMask;
					}
					DX11Context::GetDevice()->CreateBlendState(&BSDesc, &state->mBState);
				}
				void DX11BlendState::Delete(DX11BlendState * state)
				{
					state->mBState->Release();
				}
				void DX11BlendState::Bind(Graphics::Color blendfactor, unsigned int samplemas)
				{
					float blendFactor[4] = { blendfactor.r,blendfactor.g,blendfactor.b,blendfactor.a };

					DX11Context::GetContext()->OMSetBlendState(mBState, blendFactor, samplemas);
				}
			}
		}
	}
}
#endif