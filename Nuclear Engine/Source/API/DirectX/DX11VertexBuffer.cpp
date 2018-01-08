#include <API\DirectX\DX11VertexBuffer.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11Shader.h>
namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			DXGI_FORMAT GetDXDataType(DataType dataType);

			DX11VertexBuffer::DX11VertexBuffer() : VertexBuffer(nullptr), inputLayout(nullptr), stride(0), offset(0)
			{
			}

			DX11VertexBuffer::~DX11VertexBuffer()
			{
				if (VertexBuffer != nullptr)
				{
					VertexBuffer->Release();
				}

				if (inputLayout != nullptr)
				{
					inputLayout->Release();					
				}

				VertexBuffer = nullptr;
				inputLayout = nullptr;
			}

			void DX11VertexBuffer::Create(DX11VertexBuffer * buffer, VertexBufferDesc * desc)
			{
				D3D11_BUFFER_DESC VertexBufferDesc;

				if (desc->usage == BufferGPUUsage::Static) {
					VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				}
				else if (desc->usage == BufferGPUUsage::Dynamic)
				{
					VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				}
				else
				{
					VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				}

				VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				VertexBufferDesc.ByteWidth = desc->size;

				if (desc->access == BufferCPUAccess::ReadOnly)
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
				InitialData.pSysMem = desc->data;
				InitialData.SysMemPitch = sizeof(desc->data);
				InitialData.SysMemSlicePitch = sizeof(desc->data);


				DX11Context::GetDevice()->CreateBuffer(&VertexBufferDesc, &InitialData, &buffer->VertexBuffer);
			}

			void DX11VertexBuffer::Update(const void* data, unsigned int size)
			{
				DX11Context::GetContext()->UpdateSubresource(VertexBuffer, 0, 0, data, 0, 0);
			}

			void DX11VertexBuffer::Bind()
			{
				DX11Context::GetContext()->IASetInputLayout(inputLayout);
				DX11Context::GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
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

			void DX11VertexBuffer::SetInputLayout(InputLayout * layout, DX11Shader *shader)
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
					shader->VS_Buffer,
					shader->VS_Size,
					&inputLayout);

				delete[] inputElementDesc;
			}
		}
	}
}
#endif