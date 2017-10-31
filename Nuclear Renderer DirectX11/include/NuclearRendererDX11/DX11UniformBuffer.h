#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBUniformBuffer.h>

namespace NuclearRenderer {
	
	class NRDX11API DX11UniformBuffer : public NRBUniformBuffer
	{
	public:
		DX11UniformBuffer();

		void Create(const char *Nameinshader, unsigned int size, unsigned int Bindingindex) override;
		void Update(void* data, unsigned int size, unsigned int offset, unsigned int slot, NuclearEngine::ShaderType type) override;
		void Update(void* data, unsigned int size, unsigned int slot, NuclearEngine::ShaderType type) override;

		void Delete() override;
		unsigned int GetBindingIndex() override;
		const char * GetName() override;

	protected:
		ID3D11Buffer* buffer;
	};
}