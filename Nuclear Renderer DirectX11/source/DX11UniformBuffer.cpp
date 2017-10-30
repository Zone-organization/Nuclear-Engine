#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11UniformBuffer.h>

namespace NuclearRenderer {

	DX11UniformBuffer::DX11UniformBuffer()
	{
	}
	void DX11UniformBuffer::Create(const char * Nameinshader, unsigned int size, unsigned int Bindingindex)
	{
		//TODO: I suppose not necessry??
		this->name = Nameinshader;
		this->m_Bindingindex = Bindingindex;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		DX11Context::GetDevice()->CreateBuffer(&bufferDesc, NULL, &buffer);
	}
	void DX11UniformBuffer::Update(void * data, unsigned int offset, unsigned int size)
	{
		const D3D11_BOX sDstBox = { offset, 0U, 0U, offset + size, 1U, 1U };

		DX11Context::GetContext()->UpdateSubresource1(buffer, 0, &sDstBox, data, 0, 0,D3D11_COPY_DISCARD);
		
		if (VS == true)
			DX11Context::GetContext()->VSSetConstantBuffers1(vsindex, 1, &buffer, &offset, &size);

		if (PS == true)
			DX11Context::GetContext()->PSSetConstantBuffers1(psindex, 1, &buffer, &offset, &size);

		if (GS == true)
			DX11Context::GetContext()->GSSetConstantBuffers1(gsindex, 1, &buffer, &offset, &size);

	}
	void DX11UniformBuffer::Delete()
	{
		buffer->Release();
	}
	unsigned int DX11UniformBuffer::GetBindingIndex()
	{
		return m_Bindingindex;
	}
	const char * DX11UniformBuffer::GetName()
	{
		return name;
	}
}