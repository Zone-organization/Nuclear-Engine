#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11IndexBuffer.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer {
	DX11IndexBuffer::DX11IndexBuffer()
	{
	}
	void DX11IndexBuffer::Delete()
	{
		buffer.Reset();
	}

	void DX11IndexBuffer::Create(void * indices, unsigned int count)
	{
		this->count = count;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = count;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(subData));

		subData.pSysMem = indices;
		subData.SysMemPitch = 0;
		subData.SysMemSlicePitch = 0;

		DX11Context::GetDevice()->CreateBuffer(&bufferDesc, &subData, &buffer);
	}

	void DX11IndexBuffer::Bind()
	{
		DX11Context::GetContext()->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind()
	{
	}

}