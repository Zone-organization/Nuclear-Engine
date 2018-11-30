#include <Engine/Graphics/API/DirectX\DX11VertexShader.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/DirectX\DX11ConstantBuffer.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				DX11VertexShader::DX11VertexShader()
				{
					mVertexShader = nullptr;
				}

				DX11VertexShader::~DX11VertexShader()
				{
					mVertexShader = nullptr;
				}

				void DX11VertexShader::Create(DX11VertexShader* result, const BinaryShaderBlob& ShaderDesc)
				{
					if (ShaderDesc.isValid)
					{
						if (ShaderDesc.DXBC_SourceCode.Buffer == nullptr)
						{
							Log.Error("[DX11VertexShader] DirectX Bytecode \"DXBC\" Buffer is nullptr!\n");
						}
						else
						{
							result->VS_Buffer = ShaderDesc.DXBC_SourceCode.Buffer;
							result->VS_Size = ShaderDesc.DXBC_SourceCode.Size;
							// encapsulate both shaders into shader Components
							if (FAILED(DX11Context::GetDevice()->CreateVertexShader(ShaderDesc.DXBC_SourceCode.Buffer, ShaderDesc.DXBC_SourceCode.Size, 0, &result->mVertexShader)))
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
					if (shader->mVertexShader != nullptr)
					{
						shader->mVertexShader->Release();
					}
					shader->mVertexShader = nullptr;
				}
				void DX11VertexShader::SetConstantBuffer(DX11ConstantBuffer* ubuffer)
				{

				}

				void DX11VertexShader::Bind()
				{
					DX11Context::GetContext()->VSSetShader(mVertexShader, 0, 0);
				}
			}
		}
	}
}
#endif