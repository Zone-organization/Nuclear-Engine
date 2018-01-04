#include <API\DirectX\DX11Shader.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11ConstantBuffer.h>
#include <d3dcompiler.h>

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
				if (VertexShader != nullptr)
				{
					VertexShader->Release();
				}
				if (GeometryShader != nullptr)
				{
					GeometryShader->Release();
				}
				if (PixelShader != nullptr)
				{
					PixelShader->Release();
				}
				VertexShader = nullptr;
				PixelShader = nullptr;
				GeometryShader = nullptr;
			}

			void DX11Shader::Create(DX11Shader* result, BinaryShaderBlob* VertexShaderCode, BinaryShaderBlob* PixelShaderCode, BinaryShaderBlob* GeometryShaderCode)
			{	
				if (VertexShaderCode != nullptr)
				{
					if (VertexShaderCode->Language !=API::ShaderLanguage::DXBC)
					{
						Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Vertex- shaders in DirectX Bytecode \"DXBC\" language!\n");

						return;
					}
				}
				if (PixelShaderCode != nullptr)
				{
					if (PixelShaderCode->Language !=API::ShaderLanguage::DXBC)
					{
						Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Pixel- shaders in DirectX Bytecode \"DXBC\" language!\n");

						return;
					}
				}
				if (GeometryShaderCode != nullptr)
				{
					if (GeometryShaderCode->Language !=API::ShaderLanguage::DXBC)
					{
						Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Geometry- shaders in DirectX Bytecode \"DXBC\" language!\n");

						return;
					}
				}

								
				if (VertexShaderCode != nullptr)
				{	
					result->VSBLOB = VertexShaderCode->DXBC_SourceCode;
					// encapsulate both shaders into shader Components
					if (FAILED(DX11Context::GetDevice()->CreateVertexShader(result->VSBLOB.Buffer,
						result->VSBLOB.Size, 0, &result->VertexShader)))
					{
						Log->Info("[DX11Shader] Vertex Shader Creation Failed!\n");
						return;
					}
				}
				if (PixelShaderCode != nullptr)
				{
					result->PSBLOB = PixelShaderCode->DXBC_SourceCode;
					if (FAILED(DX11Context::GetDevice()->CreatePixelShader(result->PSBLOB.Buffer,
						result->PSBLOB.Size, 0, &result->PixelShader)))
					{
						Log->Info("[DX11Shader] Pixel Shader Creation Failed!\n");
						return;
					}
				}
				if (GeometryShaderCode != nullptr)
				{
					result->GSBLOB = GeometryShaderCode->DXBC_SourceCode;
					if (FAILED(DX11Context::GetDevice()->CreateGeometryShader(result->GSBLOB.Buffer,
						result->GSBLOB.Size, 0, &result->GeometryShader)))
					{
						Log->Info("[DX11Shader] Geometry Shader Creation Failed!\n");
						return;
					}
				}
				return;
			}
			unsigned int DX11Shader::GetConstantBufferSlot(DX11ConstantBuffer * ubuffer,API::ShaderType type)
			{
				ID3D11ShaderReflection* pReflector = NULL;
				D3D11_SHADER_INPUT_BIND_DESC Desc;

				if (type == API::ShaderType::Vertex)
				{
					D3DReflect(VSBLOB.Buffer,
						VSBLOB.Size,
						IID_ID3D11ShaderReflection, (void**)&pReflector);

					if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->name, &Desc)))
					{
						pReflector->Release();
						return Desc.BindPoint;
					}

				}
				else if (type == API::ShaderType::Pixel)
				{
					D3DReflect(PSBLOB.Buffer,
						PSBLOB.Size,
						IID_ID3D11ShaderReflection, (void**)&pReflector);

					if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->name, &Desc)))
					{
						return Desc.BindPoint;
					}

				}
				else if (type == API::ShaderType::Geometry)
				{
					D3DReflect(GSBLOB.Buffer,
						GSBLOB.Size,
						IID_ID3D11ShaderReflection, (void**)&pReflector);

					if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->name, &Desc)))
					{
						return Desc.BindPoint;
					}
				}

				Log->Warning(std::string("[DirectX] GetConstantBufferSlot for: \"" + std::string(ubuffer->name) + "\" failed, Returning 0 as default.\n"));
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