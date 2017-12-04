#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererBase\NRBUniformBuffer.h>
#include <NuclearRendererDX11\DX11Shader.h>
#include <NuclearCommon\Utilities\Logger.h>
#include <d3dcompiler.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine;

#pragma comment(lib,"D3DCompiler.lib")

using namespace NuclearCommon;
namespace NuclearRenderer {

	DX11Shader::DX11Shader()
	{

	}

	bool DX11Shader::Create(BinaryShaderBlob* VertexShaderCode, BinaryShaderBlob* PixelShaderCode, BinaryShaderBlob* GeometryShaderCode)
	{
		bool result = true;
		m_VSBL = VertexShaderCode;
		m_PSBL = PixelShaderCode;
		m_GSBL = GeometryShaderCode;
		if (VertexShaderCode != nullptr)
		{
			if (VertexShaderCode->Language != ShaderLanguage::DXBC)
			{
				Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Vertex- shaders in DirectX Bytecode \"DXBC\" language!\n");

				result = false;
			}
		}
		if (PixelShaderCode != nullptr)
		{
			if (PixelShaderCode->Language != ShaderLanguage::DXBC)
			{
				Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Pixel- shaders in DirectX Bytecode \"DXBC\" language!\n");

				result = false;
			}
		}
		if (GeometryShaderCode != nullptr)
		{
			if (GeometryShaderCode->Language != ShaderLanguage::DXBC)
			{
				Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all -Geometry- shaders in DirectX Bytecode \"DXBC\" language!\n");

				result = false;
			}
		}


		if (result == false)
		{
			return false;
		}

		if (m_VSBL != nullptr)
		{	// encapsulate both shaders into shader Components
			if (FAILED(DX11Context::GetDevice()->CreateVertexShader(m_VSBL->GetBufferPointer(), m_VSBL->GetBufferSize(), 0, m_vertexShader.GetAddressOf())))
			{
				Log->Info("[DX11Shader] Vertex Shader Creation Failed!\n");
				result = false;
			}

		}
		if (m_PSBL != nullptr)
		{
			if (FAILED(DX11Context::GetDevice()->CreatePixelShader(m_PSBL->GetBufferPointer(), m_PSBL->GetBufferSize(), 0, m_pixelShader.GetAddressOf())))
			{
				Log->Info("[DX11Shader] Pixel Shader Creation Failed!\n");
				result = false;
			}
		}
		if (m_GSBL != nullptr)
		{
			if (FAILED(DX11Context::GetDevice()->CreateGeometryShader(m_GSBL->GetBufferPointer(), m_GSBL->GetBufferSize(), 0, m_geometryShader.GetAddressOf())))
			{
				Log->Info("[DX11Shader] Geometry Shader Creation Failed!\n");
				result = false;
			}
		}
		if (result != false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void DX11Shader::SetUniformBuffer(NRBUniformBuffer* ubuffer, unsigned int slot, NuclearEngine::ShaderType type)
	{
		if (type == ShaderType::Vertex)
		{
			DX11Context::GetContext()->VSSetShader(m_vertexShader.Get(), 0, 0);
			DX11Context::GetContext()->VSSetConstantBuffers(slot, 1, ubuffer->GetDXBuffer());
		}
		else if (type == ShaderType::Pixel)
		{
			DX11Context::GetContext()->PSSetShader(m_pixelShader.Get(), 0, 0);
			DX11Context::GetContext()->PSSetConstantBuffers(slot, 1, ubuffer->GetDXBuffer());
		}
		else if (type == ShaderType::Geometry)
		{
			DX11Context::GetContext()->GSSetShader(m_geometryShader.Get(), 0, 0);
			DX11Context::GetContext()->GSSetConstantBuffers(slot, 1, ubuffer->GetDXBuffer());
		}

		return;
	}


	unsigned int DX11Shader::GetUniformBufferSlot(NRBUniformBuffer * ubuffer, NuclearEngine::ShaderType type)
	{
		ID3D11ShaderReflection* pReflector = NULL;
		D3D11_SHADER_INPUT_BIND_DESC Desc;

		if (type == ShaderType::Vertex)
		{
			if (m_VSBL != nullptr)
			{
				D3DReflect(m_VSBL->GetBufferPointer(),
					m_VSBL->GetBufferSize(),
					IID_ID3D11ShaderReflection, (void**)&pReflector);

				if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->GetName(), &Desc)))
				{
					pReflector->Release();
					return Desc.BindPoint;
				}
			}
		}
		else if (type == ShaderType::Pixel)
		{
			if (m_PSBL != nullptr)
			{
				D3DReflect(m_PSBL->GetBufferPointer(),
					m_PSBL->GetBufferSize(),
					IID_ID3D11ShaderReflection, (void**)&pReflector);

				if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->GetName(), &Desc)))
				{
					return Desc.BindPoint;
				}
			}
		}
		else if (type == ShaderType::Geometry)
		{
			if (m_PSBL != nullptr)
			{
				D3DReflect(m_GSBL->GetBufferPointer(),
					m_GSBL->GetBufferSize(),
					IID_ID3D11ShaderReflection, (void**)&pReflector);

				if (SUCCEEDED(pReflector->GetResourceBindingDescByName(ubuffer->GetName(), &Desc)))
				{
					return Desc.BindPoint;
				}
			}
		}
		
		Log->Warning(std::string("[DirectX] GetUniformBufferSlot for: \"" + std::string(ubuffer->GetName()) + "\" failed, Returning 0 as default.\n"));
		return 0;
	}

	void DX11Shader::Delete()
	{
		m_vertexShader.Reset();
		m_pixelShader.Reset();
		m_geometryShader.Reset();

		m_VSBL = nullptr;
		m_PSBL = nullptr;
		m_GSBL = nullptr;
	}

	void DX11Shader::Bind()
	{
		// set the shader Components
		if (m_vertexShader != nullptr)
		{
			DX11Context::GetContext()->VSSetShader(m_vertexShader.Get(), 0, 0);
		}
		if (m_pixelShader != nullptr)
		{
			DX11Context::GetContext()->PSSetShader(m_pixelShader.Get(), 0, 0);
		}
		if (m_geometryShader != nullptr)
		{
			DX11Context::GetContext()->GSSetShader(m_geometryShader.Get(), 0, 0);
		}
	}

	void DX11Shader::Unbind()
	{

	}

	unsigned int DX11Shader::GetGLShaderID()
	{
		return 0;
	}

	void * DX11Shader::GetDXBufferPointer()
	{
		if (m_vertexShader != nullptr)
		{
			return (void*)m_VSBL->GetBufferPointer();
		}

		return 0;
	}

	unsigned long DX11Shader::GetDXBufferSize()
	{
		if (m_vertexShader != nullptr)
		{
			return m_VSBL->GetBufferSize();
		}

		return 0;

	}

	void DX11Shader::CheckShaderErrors(ID3D10Blob* Shader)
	{
		char* compileErrors;

		// Get a pointer to the error message text Buffer.
		compileErrors = (char*)(Shader->GetBufferPointer());

		Log->Info(compileErrors);

		// Release the error message.
		Shader->Release();
		Shader = 0;

		return;
	}
}