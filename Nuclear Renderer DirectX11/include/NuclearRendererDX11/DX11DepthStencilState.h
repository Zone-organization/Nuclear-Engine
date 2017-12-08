#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBDepthStencilState.h>

namespace NuclearRenderer {

	class NRDX11API DX11DepthStencilState : public NRBDepthStencilState
	{
	public:
		DX11DepthStencilState();

		void Create(NuclearEngine::DepthStencilStateDesc type) override;

		void Delete() override;

		void Bind() override;

		void Unbind() override;
	protected:
		ComPtr<ID3D11DepthStencilState> DS_State;
		NuclearEngine::DepthStencilStateDesc *desc;
		unsigned int count;
	};
}