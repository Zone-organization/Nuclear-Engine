#pragma once
#include <Engine/Graphics/API/DirectX\DX11RasterizerState.h>

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
				DX11RasterizerState::DX11RasterizerState()
				{
					RS_State = nullptr;
				}
				DX11RasterizerState::~DX11RasterizerState()
				{
					RS_State = nullptr;
				}
				void DX11RasterizerState::Create(DX11RasterizerState * state, const RasterizerStateDesc & type)
				{
					D3D11_RASTERIZER_DESC RSDesc;
					ZeroMemory(&RSDesc, sizeof(D3D11_RASTERIZER_DESC));

					RSDesc.FillMode = (D3D11_FILL_MODE)type.FillMode;
					RSDesc.CullMode = (D3D11_CULL_MODE)type.CullMode;
					RSDesc.FrontCounterClockwise = type.FrontCounterClockwise;
					RSDesc.DepthBias = type.DepthBias;
					RSDesc.DepthBiasClamp = type.DepthBiasClamp;
					RSDesc.SlopeScaledDepthBias = type.SlopeScaledDepthBias;
					RSDesc.DepthClipEnable = type.DepthClipEnable;
					RSDesc.ScissorEnable = type.ScissorEnable;
					RSDesc.MultisampleEnable = type.MultisampleEnable;
					RSDesc.AntialiasedLineEnable = type.AntialiasedLineEnable;

					DX11Context::GetDevice()->CreateRasterizerState(&RSDesc, &state->RS_State);
				}
				void DX11RasterizerState::Delete(DX11RasterizerState * state)
				{
					state->RS_State->Release();
				}
				void DX11RasterizerState::Bind()
				{
					DX11Context::GetContext()->RSSetState(RS_State);
				}
			}
		}
	}
}
#endif