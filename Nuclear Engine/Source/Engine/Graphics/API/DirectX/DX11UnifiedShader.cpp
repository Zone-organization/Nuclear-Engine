#include <Engine/Graphics/API/DirectX\DX11UnifiedShader.h>

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
				DX11UnifiedShader::DX11UnifiedShader()
				{
					VertexShader = nullptr;
				}

				DX11UnifiedShader::~DX11UnifiedShader()
				{
					VertexShader = nullptr;
				}

				void DX11UnifiedShader::Create(DX11UnifiedShader* result, const BinaryShaderBlob& VS_Desc, const BinaryShaderBlob& PS_Desc)
				{
					if (VS_Desc.isValid)
					{
						if (VS_Desc.DXBC_SourceCode.Buffer == nullptr)
						{
							Log.Error("[DX11UnifiedShader] DirectX Bytecode \"DXBC\" Buffer is nullptr!\n");
						}
						else
						{
							result->VS_Buffer = VS_Desc.DXBC_SourceCode.Buffer;
							result->VS_Size = VS_Desc.DXBC_SourceCode.Size;
							result->Reflection = VS_Desc.Reflection;
							// encapsulate both shaders into shader Components
							if (FAILED(DX11Context::GetDevice()->CreateVertexShader(VS_Desc.DXBC_SourceCode.Buffer, VS_Desc.DXBC_SourceCode.Size, 0, &result->VertexShader)))
							{
								Log.Info("[DX11UnifiedShader] Vertex Shader Creation Failed!\n");
								return;
							}
						}
					}
					if (PS_Desc.isValid)
					{
						if (PS_Desc.DXBC_SourceCode.Buffer == nullptr)
						{
							Log.Error("[DX11PixelShader] DirectX Bytecode \"DXBC\" Buffer is nullptr!\n");
						}
						else
						{
							result->Reflection = PS_Desc.Reflection;
							if (FAILED(DX11Context::GetDevice()->CreatePixelShader(PS_Desc.DXBC_SourceCode.Buffer, PS_Desc.DXBC_SourceCode.Size, 0, &result->PixelShader)))
							{
								Log.Info("[DX11PixelShader] Pixel Shader Creation Failed!\n");
								return;
							}
						}
					}
					return;
				}
				void DX11UnifiedShader::Delete(DX11UnifiedShader * shader)
				{
					if (shader->VertexShader != nullptr)
					{
						shader->VertexShader->Release();
					}
					shader->VertexShader = nullptr;
				}
				void DX11UnifiedShader::SetConstantBuffer(DX11ConstantBuffer* ubuffer)
				{
					DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);
					DX11Context::GetContext()->VSSetConstantBuffers(Reflection.ConstantBuffers[ubuffer->GetName()].BindingSlot, 1, ubuffer->GetBufferPtr());
				}

				void DX11UnifiedShader::Bind()
				{
					DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);
				}
			}
		}
	}
}
#endif