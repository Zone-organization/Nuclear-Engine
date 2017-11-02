#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBVertexBuffer.h>

namespace NuclearRenderer {
	enum class DataType;

	class NRDX11API DX11VertexBuffer : public NRBVertexBuffer
	{
	public:
		DX11VertexBuffer();

		void Delete() override;

		void Create(const void* data, unsigned int size, NuclearEngine::BufferGPUUsage usage, NuclearEngine::BufferCPUAccess access) override;

		void Update(const void* data, unsigned int size) override;

		void SetInputLayout(NRBInputLayout * layout, NRBShader *shader) override;

		void Bind() override;
		void Unbind() override;
	private:
		ComPtr<ID3D11Buffer> VertexBuffer;
		ComPtr<ID3D11InputLayout> inputLayout;

		unsigned int offset, stride;
		DXGI_FORMAT GetDXDataType(DataType dataType);
	};
}