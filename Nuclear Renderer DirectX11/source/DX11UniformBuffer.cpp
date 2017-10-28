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
		DX11Context::GetContext()->UpdateSubresource(buffer, 0, NULL, data, 0, 0);

		if(VS == true)
			DX11Context::GetContext()->VSSetConstantBuffers(vsindex, 1, &buffer);
		
		if(PS == true)
			DX11Context::GetContext()->PSSetConstantBuffers(psindex, 1, &buffer);

		if(GS == true)
			DX11Context::GetContext()->GSSetConstantBuffers(gsindex, 1, &buffer);

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