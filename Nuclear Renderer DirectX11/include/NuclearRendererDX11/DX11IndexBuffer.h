#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBIndexBuffer.h>

namespace NuclearRenderer {

	class NRDX11API DX11IndexBuffer : public NRBIndexBuffer
	{
	public:
		DX11IndexBuffer();

		void Delete() override;

		void Create(void* indices, unsigned int count) override;

		void Bind() override;
		void Unbind() override;
	protected:
		ComPtr<ID3D11Buffer> buffer;
		unsigned int count;
	};
}