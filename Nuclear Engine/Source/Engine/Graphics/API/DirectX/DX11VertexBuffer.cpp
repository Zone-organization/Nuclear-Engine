#include <Engine/Graphics/API/DirectX\DX11VertexBuffer.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/DirectX\DX11VertexShader.h>
#include <Engine/Graphics/API/InputLayout.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{

				DX11VertexBuffer::DX11VertexBuffer() : mVBuffer(nullptr), mInputLayout(nullptr), mStride(0), mOffset(0)
				{
				}

				DX11VertexBuffer::~DX11VertexBuffer()
				{
					mVBuffer = nullptr;
					mInputLayout = nullptr;
				}

				void DX11VertexBuffer::Create(DX11VertexBuffer * buffer, const VertexBufferDesc& desc)
				{
					buffer->mOffset = 0;
					D3D11_BUFFER_DESC VertexBufferDesc;

					if (desc.UsageType == BufferUsage::Static) {
						VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
						VertexBufferDesc.CPUAccessFlags = 0;
					}
					else if (desc.UsageType == BufferUsage::Dynamic)
					{
						VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
						VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
					}
					else
					{
						VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
						VertexBufferDesc.CPUAccessFlags = 0;
					}

					VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					VertexBufferDesc.ByteWidth = desc.Size;
					VertexBufferDesc.MiscFlags = 0;
					VertexBufferDesc.StructureByteStride = 0;

					if (desc.Data != NULL)
					{
						D3D11_SUBRESOURCE_DATA InitialData;
						InitialData.pSysMem = desc.Data;
						InitialData.SysMemPitch = 0;
						InitialData.SysMemSlicePitch = 0;
						DX11Context::GetDevice()->CreateBuffer(&VertexBufferDesc, &InitialData, &buffer->mVBuffer);

					}
					else
					{
						DX11Context::GetDevice()->CreateBuffer(&VertexBufferDesc, NULL, &buffer->mVBuffer);
					}
				}

				void DX11VertexBuffer::Delete(DX11VertexBuffer * buffer)
				{
					if (buffer->mVBuffer != nullptr)
					{
						buffer->mVBuffer->Release();
					}

					if (buffer->mInputLayout != nullptr)
					{
						buffer->mInputLayout->Release();
					}

					buffer->mVBuffer = nullptr;
					buffer->mInputLayout = nullptr;
				}

				void DX11VertexBuffer::Update(const void* data, unsigned int size)
				{
					DX11Context::GetContext()->UpdateSubresource(mVBuffer, 0, 0, data, 0, 0);
				}

				void DX11VertexBuffer::Bind()
				{
					DX11Context::GetContext()->IASetInputLayout(mInputLayout);
					DX11Context::GetContext()->IASetVertexBuffers(0, 1, &mVBuffer, &mStride, &mOffset);
				}
				void * DX11VertexBuffer::Map()
				{
					D3D11_MAPPED_SUBRESOURCE mappedSubResource;
					ZeroMemory(&mappedSubResource, sizeof(mappedSubResource));

					DX11Context::GetContext()->Map(mVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
					return mappedSubResource.pData;
				}
				void DX11VertexBuffer::Unmap()
				{
					DX11Context::GetContext()->Unmap(mVBuffer, 0);
				}
				unsigned int GetDXDataTypeSizeInBytes(Format dataType)
				{
					switch (dataType)
					{
					case Format::R32_FLOAT:
					{	return 4;	}
					case Format::R32G32_FLOAT:
					{	return 8;	}
					case Format::R32G32B32_FLOAT:
					{	return 12;	}
					case Format::R32G32B32A32_FLOAT:
					{	return 16;	}
					case Format::R8G8B8A8_UNORM:
					{	return 4;	}
					default:
						return -1;
					}
				}
				void DX11VertexBuffer::SetInputLayout(InputLayout * layout, DX11VertexShader * shader)
				{
					D3D11_INPUT_ELEMENT_DESC* inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->GetBufferElement().size()];

					for (size_t i = 0; i < layout->GetBufferElement().size(); i++)
					{
						inputElementDesc[i].SemanticName = layout->GetBufferElement()[i].name;
						inputElementDesc[i].SemanticIndex = layout->GetBufferElement()[i].index;
						inputElementDesc[i].Format = (DXGI_FORMAT)layout->GetBufferElement()[i].format;
						inputElementDesc[i].InputSlot = 0;
						inputElementDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
						inputElementDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
						inputElementDesc[i].InstanceDataStepRate = 0;

						mStride = mStride + GetDXDataTypeSizeInBytes(layout->GetBufferElement()[i].format);
					}

					DX11Context::GetDevice()->CreateInputLayout(inputElementDesc,
						(unsigned int)layout->GetBufferElement().size(),
						shader->VS_Buffer,
						shader->VS_Size,
						&mInputLayout);
					delete[] inputElementDesc;
				}
			}
			
		}
	}
}
#endif