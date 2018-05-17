#include <Engine/Graphics/API/DirectX\DX11IndexBuffer.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Context.h>

namespace NuclearEngine
{
	namespace Graphics 
	{
		namespace API
		{
			namespace DirectX
			{

				DX11IndexBuffer::DX11IndexBuffer()
				{
					buffer = nullptr;
				}
				DX11IndexBuffer::~DX11IndexBuffer()
				{
					buffer = nullptr;
				}

				void DX11IndexBuffer::Create(DX11IndexBuffer* result, const void * indices, unsigned int count)
				{
					D3D11_BUFFER_DESC bufferDesc;
					ZeroMemory(&bufferDesc, sizeof(bufferDesc));

					bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
					bufferDesc.ByteWidth = count * sizeof(unsigned int);
					bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bufferDesc.CPUAccessFlags = 0;
					bufferDesc.MiscFlags = 0;

					D3D11_SUBRESOURCE_DATA subData;
					ZeroMemory(&subData, sizeof(subData));

					subData.pSysMem = indices;
					subData.SysMemPitch = 0;
					subData.SysMemSlicePitch = 0;

					DX11Context::GetDevice()->CreateBuffer(&bufferDesc, &subData, &result->buffer);
				}

				void DX11IndexBuffer::Delete(DX11IndexBuffer * ibuffer)
				{
					if (ibuffer->buffer != nullptr)
					{
						ibuffer->buffer->Release();
					}

					ibuffer->buffer = nullptr;
				}

				void DX11IndexBuffer::Bind()
				{
					DX11Context::GetContext()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
				}
			}
		}
	}
}

#endif