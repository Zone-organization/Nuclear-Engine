#include <API\DirectX\DX11PixelShader.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11ConstantBuffer.h>
#include <API\Shader_Types.h>

namespace NuclearEngine
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
					if (sdesc->Language != API::ShaderLanguage::DXBC)
					{
						Log.Error("[DX11PixelShader] DirectX 11 Renderer Backend expects all Pixel shaders in DirectX Bytecode \"DXBC\" language!\n");
					}
					else
					{
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
			unsigned int DX11PixelShader::GetConstantBufferSlot(DX11ConstantBuffer * ubuffer, API::ShaderType type)
			{
				return 0;
			}

			void DX11PixelShader::SetConstantBuffer(DX11ConstantBuffer* ubuffer, API::ShaderType type)
			{
				DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
				DX11Context::GetContext()->PSSetConstantBuffers(this->GetConstantBufferSlot(ubuffer, type), 1, ubuffer->GetBufferPtr());
			}

			void DX11PixelShader::Bind()
			{
				DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
			}
		}
	}
}

#endif