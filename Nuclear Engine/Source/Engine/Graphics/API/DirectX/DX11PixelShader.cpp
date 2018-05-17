#include <Engine/Graphics/API/DirectX\DX11PixelShader.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/DirectX\DX11ConstantBuffer.h>
#include <Engine/Graphics/API/Shader_Types.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				DX11PixelShader::DX11PixelShader()
				{
					PixelShader = nullptr;
				}

				DX11PixelShader::~DX11PixelShader()
				{
					PixelShader = nullptr;
				}

				void DX11PixelShader::Create(DX11PixelShader* result, BinaryShaderBlob* sdesc)
				{
					if (sdesc->Finished)
					{
						if (sdesc->DXBC_SourceCode.Buffer == nullptr)
						{
							Log.Error("[DX11PixelShader] DirectX Bytecode \"DXBC\" Buffer is nullptr!\n");
						}
						else
						{
							result->Reflection = sdesc->Reflection;
							if (FAILED(DX11Context::GetDevice()->CreatePixelShader(sdesc->DXBC_SourceCode.Buffer, sdesc->DXBC_SourceCode.Size, 0, &result->PixelShader)))
							{
								Log.Info("[DX11PixelShader] Pixel Shader Creation Failed!\n");
								return;
							}
						}
					}
					return;
				}
				void DX11PixelShader::Delete(DX11PixelShader * shader)
				{
					if (shader->PixelShader != nullptr)
					{
						shader->PixelShader->Release();
					}
					shader->PixelShader = nullptr;
				}
				void DX11PixelShader::SetConstantBuffer(DX11ConstantBuffer* ubuffer)
				{
					DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
					DX11Context::GetContext()->PSSetConstantBuffers(Reflection.ConstantBuffers[ubuffer->GetName()].BindingSlot, 1, ubuffer->GetBufferPtr());
				}

				void DX11PixelShader::Bind()
				{
					DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
				}
			}
		}
	}
}

#endif