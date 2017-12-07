#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11VertexBuffer.h>
#include <NuclearRendererDX11\DX11Shader.h>
#include <NuclearRendererBase/NRBInputLayout.h>
#include <NuclearCommon\Common_API_Types.h>
#include "comdef.h"
using namespace NuclearEngine;
namespace NuclearRenderer
{
	DXGI_FORMAT GetDXDataType(DataType dataType);

	DX11VertexBuffer::DX11VertexBuffer() : VertexBuffer(nullptr), stride(0), offset(0)
	{
	}

	void DX11VertexBuffer::Delete()
	{
		VertexBuffer.Reset();
		inputLayout.Reset();
	}
	
	void DX11VertexBuffer::Create(const void* data, unsigned int size, BufferGPUUsage usage, BufferCPUAccess access)
	{
		D3D11_BUFFER_DESC VertexBufferDesc;

		if (usage == BufferGPUUsage::Static) {
			VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if (usage == BufferGPUUsage::Dynamic)
		{
			VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else
		{
			VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		}

		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.ByteWidth = size;

		if (access == BufferCPUAccess::ReadOnly)
		{
			VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		}
		else
		{
			VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA InitialData;
		InitialData.pSysMem = data;
		InitialData.SysMemPitch = sizeof(data);
		InitialData.SysMemSlicePitch = sizeof(data);


		if (FAILED(DX11Context::GetDevice()->CreateBuffer(&VertexBufferDesc, &InitialData, VertexBuffer.GetAddressOf())))
		{
			return;
		}
		return;

	}

	void DX11VertexBuffer::Update(const void* data, unsigned int size)
	{
		DX11Context::GetContext()->UpdateSubresource(VertexBuffer.Get(), 0, 0, data, 0, 0);
	}

	void DX11VertexBuffer::Bind()
	{
		DX11Context::GetContext()->IASetInputLayout(inputLayout.Get());
		DX11Context::GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
	}

	void DX11VertexBuffer::Unbind()
	{
		return;
	}

	DXGI_FORMAT GetDXDataType(DataType dataType)
	{
		switch (dataType)
		{
		case DataType::Float:
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			break;
		}
		case DataType::Float2:
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
			break;
		}
		case DataType::Float3:
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}
		case DataType::Float4:
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		default:
			break;
		}

		return DXGI_FORMAT();
	}
	unsigned int GetDXDataTypeSizeInBytes(DataType dataType)
	{
		switch (dataType)
		{
		case DataType::Float:
		{
			return 4;
			break;
		}
		case DataType::Float2:
		{
			return 8;
			break;
		}
		case DataType::Float3:
		{
			return 12;
			break;
		}
		case DataType::Float4:
		{
			return 16;
			break;
		}
		default:
			return -1;
			break;
		}
	}

	void DX11VertexBuffer::SetInputLayout(NRBInputLayout * layout, NRBShader *shader)
	{
		D3D11_INPUT_ELEMENT_DESC* inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->GetBufferElement().size()];

		for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
		{
			inputElementDesc[i].SemanticName = layout->GetBufferElement()[i].name;
			inputElementDesc[i].SemanticIndex = layout->GetBufferElement()[i].index;
			inputElementDesc[i].Format = GetDXDataType(layout->GetBufferElement()[i].dataType);
			inputElementDesc[i].InputSlot = 0;
			inputElementDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputElementDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElementDesc[i].InstanceDataStepRate = 0;

			stride = stride + GetDXDataTypeSizeInBytes(layout->GetBufferElement()[i].dataType);
		}

		DX11Context::GetDevice()->CreateInputLayout(inputElementDesc,
			(unsigned int)layout->GetBufferElement().size(),
			shader->GetDXBufferPointer(),
			shader->GetDXBufferSize(),
			&inputLayout);

		delete[] inputElementDesc;
	}
}