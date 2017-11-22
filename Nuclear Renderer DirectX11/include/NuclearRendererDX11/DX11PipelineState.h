#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBPipelineState.h>

namespace NuclearRenderer {

	class NRDX11API DX11PipelineState : public NRBPipelineState
	{
	public:
		DX11PipelineState();

		void Create(NuclearEngine::PipelineStateDesc type) override;

		void Delete() override;

		void Bind_DepthStencil() override;

		void Unbind_DepthStencil() override;
	protected:
		ComPtr<ID3D11DepthStencilState> DS_State;
		NuclearEngine::PipelineStateDesc *desc;
		unsigned int count;
	};
}