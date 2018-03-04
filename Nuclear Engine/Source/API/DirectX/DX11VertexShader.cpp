#include <API\DirectX\DX11VertexShader.h>

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
			DX11VertexShader::DX11VertexShader()
			{
				VertexShader = nullptr;
			}

			DX11VertexShader::~DX11VertexShader()
			{
				VertexShader = nullptr;
			}

			void DX11VertexShader::Create(DX11VertexShader* result, BinaryShaderBlob* sdesc)
			{
				if (sdesc->Finished)
				{
					if (sdesc->DXBC_SourceCode.Buffer == nullptr)
					{
						Log.Error("[DX11VertexShader] DirectX Bytecode \"DXBC\" Buffer is nullptr!\n");
					}
					else 
					{
						result->VS_Buffer = sdesc->DXBC_SourceCode.Buffer;
						result->VS_Size = sdesc->DXBC_SourceCode.Size;
						result->Reflection = sdesc->Reflection;
						// encapsulate both shaders into shader Components
						if (FAILED(DX11Context::GetDevice()->CreateVertexShader(sdesc->DXBC_SourceCode.Buffer,	sdesc->DXBC_SourceCode.Size, 0, &result->VertexShader)))
						{
							Log.Info("[DX11VertexShader] Vertex Shader Creation Failed!\n");
							return;
						}
					}
				}	
				return;
			}
			void DX11VertexShader::Delete(DX11VertexShader * shader)
			{
				if (shader->VertexShader != nullptr)
				{
					shader->VertexShader->Release();
				}
				shader->VertexShader = nullptr;
			}
			void DX11VertexShader::SetConstantBuffer(DX11ConstantBuffer* ubuffer)
			{
				DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);
				DX11Context::GetContext()->VSSetConstantBuffers(Reflection.ConstantBuffers[ubuffer->GetName()].BindingSlot, 1, ubuffer->GetBufferPtr());
			}

			void DX11VertexShader::Bind()
			{
				DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);		
			}
		}
	}
}

#endif