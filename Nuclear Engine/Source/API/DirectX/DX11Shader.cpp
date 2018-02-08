#include <API\DirectX\DX11Shader.h>

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
			DX11Shader::DX11Shader()
			{
				VertexShader = nullptr;
				PixelShader = nullptr;
				GeometryShader = nullptr;
			}

			DX11Shader::~DX11Shader()
			{				
				VertexShader = nullptr;
				PixelShader = nullptr;
				GeometryShader = nullptr;
			}

			void DX11Shader::Create(DX11Shader* result, ShaderDesc* sdesc)
			{	
				if (sdesc->VertexShaderCode.Finished)
				{
					if (sdesc->VertexShaderCode.Language !=API::ShaderLanguage::DXBC)
					{
						Log.Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Vertex- shaders in DirectX Bytecode \"DXBC\" language!\n");
					}
					else {
						result->VS_Buffer = sdesc->VertexShaderCode.DXBC_SourceCode.Buffer;
						result->VS_Size = sdesc->VertexShaderCode.DXBC_SourceCode.Size;

						// encapsulate both shaders into shader Components
						if (FAILED(DX11Context::GetDevice()->CreateVertexShader(sdesc->VertexShaderCode.DXBC_SourceCode.Buffer,
							sdesc->VertexShaderCode.DXBC_SourceCode.Size, 0, &result->VertexShader)))
						{
							Log.Info("[DX11Shader] Vertex Shader Creation Failed!\n");
							return;
						}
					}
				}
				if (sdesc->PixelShaderCode.Finished)
				{
					if (sdesc->PixelShaderCode.Language !=API::ShaderLanguage::DXBC)
					{
						Log.Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Pixel- shaders in DirectX Bytecode \"DXBC\" language!\n");
					}
					else
					{
						if (FAILED(DX11Context::GetDevice()->CreatePixelShader(sdesc->PixelShaderCode.DXBC_SourceCode.Buffer,
							sdesc->PixelShaderCode.DXBC_SourceCode.Size, 0, &result->PixelShader)))
						{
							Log.Info("[DX11Shader] Pixel Shader Creation Failed!\n");
							return;
						}
					}					
				}
				if (sdesc->GeometryShaderCode.Finished)
				{
					if (sdesc->GeometryShaderCode.Language !=API::ShaderLanguage::DXBC)
					{
						Log.Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Geometry- shaders in DirectX Bytecode \"DXBC\" language!\n");
					}
					else
					{
						if (FAILED(DX11Context::GetDevice()->CreateGeometryShader(sdesc->GeometryShaderCode.DXBC_SourceCode.Buffer,
							sdesc->GeometryShaderCode.DXBC_SourceCode.Size, 0, &result->GeometryShader)))
						{
							Log.Info("[DX11Shader] Geometry Shader Creation Failed!\n");
							return;
						}
					}
				}
												
			
				return;
			}
			void DX11Shader::Delete(DX11Shader * shader)
			{
				if (shader->VertexShader != nullptr)
				{
					shader->VertexShader->Release();
				}
				if (shader->GeometryShader != nullptr)
				{
					shader->GeometryShader->Release();
				}
				if (shader->PixelShader != nullptr)
				{
					shader->PixelShader->Release();
				}
				shader->VertexShader = nullptr;
				shader->PixelShader = nullptr;
				shader->GeometryShader = nullptr;
			}
			unsigned int DX11Shader::GetConstantBufferSlot(DX11ConstantBuffer * ubuffer,API::ShaderType type)
			{
				
				return 0;
			}

			void DX11Shader::SetConstantBuffer(DX11ConstantBuffer* ubuffer,API::ShaderType type)
			{
				if (type ==API::ShaderType::Vertex)
				{
					DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);
					DX11Context::GetContext()->VSSetConstantBuffers(this->GetConstantBufferSlot(ubuffer,type), 1, &ubuffer->buffer);
				}
				else if (type ==API::ShaderType::Pixel)
				{
					DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
					DX11Context::GetContext()->PSSetConstantBuffers(this->GetConstantBufferSlot(ubuffer, type), 1, &ubuffer->buffer);
				}
				else if (type ==API::ShaderType::Geometry)
				{
					DX11Context::GetContext()->GSSetShader(GeometryShader, 0, 0);
					DX11Context::GetContext()->GSSetConstantBuffers(this->GetConstantBufferSlot(ubuffer, type), 1, &ubuffer->buffer);
				}

				return;
			}
			
			void DX11Shader::Bind()
			{
				// set the shader Components
				if (VertexShader != nullptr)
				{
					DX11Context::GetContext()->VSSetShader(VertexShader, 0, 0);
				}
				if (PixelShader != nullptr)
				{
					DX11Context::GetContext()->PSSetShader(PixelShader, 0, 0);
				}
				if (GeometryShader != nullptr)
				{
					DX11Context::GetContext()->GSSetShader(GeometryShader, 0, 0);
				}
			}
		}
	}
}

#endif