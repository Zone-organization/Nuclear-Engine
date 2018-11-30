#include <Engine/Graphics/API/DirectX\DX11ConstantBuffer.h>

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
				DX11ConstantBuffer::DX11ConstantBuffer()
				{
					mName = "";
					mCBuffer = nullptr;
				}
				DX11ConstantBuffer::~DX11ConstantBuffer()
				{
					mCBuffer = nullptr;
				}
				void DX11ConstantBuffer::Create(DX11ConstantBuffer* result, const char * Nameinshader, unsigned int size)
				{
					int remainder = size % 16;
					if (remainder != 0)
					{
						Log.Warning("[DX11ConstantBuffer] The size of mCBuffer isn't a multiple of 16 which can cause many unexpected problems! \n");
					}

					result->mName = Nameinshader;

					D3D11_BUFFER_DESC bufferDesc;
					ZeroMemory(&bufferDesc, sizeof(bufferDesc));

					bufferDesc.Usage = D3D11_USAGE_DEFAULT;
					bufferDesc.ByteWidth = size;
					bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					bufferDesc.CPUAccessFlags = 0;
					bufferDesc.MiscFlags = 0;

					DX11Context::GetDevice()->CreateBuffer(&bufferDesc, NULL, &result->mCBuffer);
				}

				void DX11ConstantBuffer::Delete(DX11ConstantBuffer * cbuffer)
				{
					if (cbuffer->mCBuffer != nullptr)
					{
						cbuffer->mCBuffer->Release();
					}

					cbuffer->mCBuffer = nullptr;
				}

				void DX11ConstantBuffer::Update(const void * data, unsigned int size)
				{
					DX11Context::GetContext()->UpdateSubresource(mCBuffer, 0, NULL, data, 0, 0);
				}

				void DX11ConstantBuffer::VSBind(Uint8 slot)
				{
					DX11Context::GetContext()->VSSetConstantBuffers(slot, 1, &mCBuffer);
				}

				void DX11ConstantBuffer::PSBind(Uint8 slot)
				{
					DX11Context::GetContext()->PSSetConstantBuffers(slot, 1, &mCBuffer);

				}
				void DX11ConstantBuffer::GSBind(Uint8 slot)
				{
					DX11Context::GetContext()->GSSetConstantBuffers(slot, 1, &mCBuffer);
				}
			}
		}
	}
}
#endif