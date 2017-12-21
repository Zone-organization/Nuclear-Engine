#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11ConstantBuffer.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer {

	DX11ConstantBuffer::DX11ConstantBuffer()
	{
	}
	void DX11ConstantBuffer::Create(const char * Nameinshader, unsigned int size)
	{
		//TODO: I suppose not necessry??
		this->name = Nameinshader;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		DX11Context::GetDevice()->CreateBuffer(&bufferDesc, NULL, &buffer);
	}

	void DX11ConstantBuffer::Update(const void * data, unsigned int size)
	{
		DX11Context::GetContext()->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
	}
	void DX11ConstantBuffer::Delete()
	{
		buffer->Release();
	}
	unsigned int DX11ConstantBuffer::GetBindingIndex()
	{
		return m_Bindingindex;
	}
	const char * DX11ConstantBuffer::GetName()
	{
		return name;
	}
	ID3D11Buffer * const * DX11ConstantBuffer::GetDXBuffer()
	{
		return &buffer;
	}
}