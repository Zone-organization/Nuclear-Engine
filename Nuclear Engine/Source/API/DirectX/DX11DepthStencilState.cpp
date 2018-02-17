#include <API\DirectX\DX11DepthStencilState.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\RenderStates_Types.h>
#include <API\DirectX\DX11Context.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			D3D11_COMPARISON_FUNC GetDXComparisonFunc(Comparison_Func func)
			{
				switch (func)
				{
				case Comparison_Func::NEVER: return D3D11_COMPARISON_NEVER;
				case Comparison_Func::LESS: return D3D11_COMPARISON_LESS;
				case Comparison_Func::EQUAL: return D3D11_COMPARISON_EQUAL;
				case Comparison_Func::LESS_EQUAL: return D3D11_COMPARISON_LESS_EQUAL;
				case Comparison_Func::GREATER: return D3D11_COMPARISON_GREATER;
				case Comparison_Func::NOT_EQUAL: return D3D11_COMPARISON_NOT_EQUAL;
				case Comparison_Func::GREATER_EQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
				case Comparison_Func::ALWAYS: return D3D11_COMPARISON_ALWAYS;

				default: return D3D11_COMPARISON_ALWAYS;
				}
			}

			D3D11_STENCIL_OP GetDXStencilOP(Stencil_OP func)
			{
				switch (func)
				{
				case Stencil_OP::KEEP: return D3D11_STENCIL_OP_KEEP;
				case Stencil_OP::ZERO: return D3D11_STENCIL_OP_ZERO;
				case Stencil_OP::REPLACE: return D3D11_STENCIL_OP_REPLACE;
				case Stencil_OP::INCREASE_SAT: return D3D11_STENCIL_OP_INCR_SAT;
				case Stencil_OP::DECREASE_SAT: return D3D11_STENCIL_OP_DECR_SAT;
				case Stencil_OP::INVERT: return D3D11_STENCIL_OP_INVERT;
				case Stencil_OP::INCREASE: return D3D11_STENCIL_OP_INCR;
				case Stencil_OP::DECREASE: return D3D11_STENCIL_OP_DECR;

				default: return D3D11_STENCIL_OP_KEEP;
				}
			}

			DX11DepthStencilState::DX11DepthStencilState()
			{
				DS_State = nullptr;
			}

			DX11DepthStencilState::~DX11DepthStencilState()
			{
				DS_State = nullptr;
			}

			void DX11DepthStencilState::Create(DX11DepthStencilState* result, const DepthStencilStateDesc& type)
			{

				D3D11_DEPTH_STENCIL_DESC DSDesc;
				ZeroMemory(&DSDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));


				DSDesc.DepthEnable = type.DepthEnabled;

				if (type.DepthMaskEnabled == true)
				{
					DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				}
				else
				{
					DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				}

				DSDesc.DepthFunc = GetDXComparisonFunc(type.DepthFunc);

				DSDesc.StencilEnable = type.StencilEnabled;
				DSDesc.StencilReadMask = type.StencilReadMask;
				DSDesc.StencilWriteMask = type.StencilWriteMask;

				DSDesc.FrontFace.StencilFunc = GetDXComparisonFunc(type.StencilFrontFace.StencilFunc);
				DSDesc.FrontFace.StencilPassOp = GetDXStencilOP(type.StencilFrontFace.StencilPassOp);
				DSDesc.FrontFace.StencilDepthFailOp = GetDXStencilOP(type.StencilFrontFace.StencilDepthFailOp);
				DSDesc.FrontFace.StencilFailOp = GetDXStencilOP(type.StencilFrontFace.StencilFailOp);

				DSDesc.BackFace.StencilFunc = GetDXComparisonFunc(type.StencilBackFace.StencilFunc);
				DSDesc.BackFace.StencilPassOp = GetDXStencilOP(type.StencilBackFace.StencilPassOp);
				DSDesc.BackFace.StencilDepthFailOp = GetDXStencilOP(type.StencilBackFace.StencilDepthFailOp);
				DSDesc.BackFace.StencilFailOp = GetDXStencilOP(type.StencilBackFace.StencilFailOp);


				DX11Context::GetDevice()->CreateDepthStencilState(&DSDesc, &result->DS_State);

			}

			void DX11DepthStencilState::Delete(DX11DepthStencilState * state)
			{
				if (state->DS_State != nullptr)
				{
					state->DS_State->Release();
				}

				state->DS_State = nullptr;
			}

			void DX11DepthStencilState::Bind()
			{
				DX11Context::GetContext()->OMSetDepthStencilState(DS_State, 0);
			}
		}
	}
}

#endif