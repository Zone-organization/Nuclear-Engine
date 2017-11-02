#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBIndexBuffer.h>

namespace NuclearRenderer {

	class NRDX11API DX11IndexBuffer : public NRBIndexBuffer
	{
	public:
		DX11IndexBuffer();

		void Create(const void* data, unsigned int size) override;

		void Delete() override;

		void Bind() override;

		void Unbind() override;
	protected:
		ComPtr<ID3D11Buffer> buffer;
		unsigned int count;
	};
}