#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBConstantBuffer.h>

namespace NuclearRenderer {
	
	class NRDX11API DX11ConstantBuffer : public NRBConstantBuffer
	{
	public:
		DX11ConstantBuffer();

		void Create(const char *Nameinshader, unsigned int size) override;
		void Update(const void* data, unsigned int size) override;

		void Delete() override;
		unsigned int GetBindingIndex() override;
		const char * GetName() override;

		ID3D11Buffer* const *GetDXBuffer();
	protected:
		ID3D11Buffer* buffer;
	};
}