#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11Shader.h>
#include <NuclearCommon\Utilities\Logger.h>
#include <d3dcompiler.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine;

#pragma comment(lib,"d3dcompiler.lib")

using namespace NuclearCommon;
namespace NuclearRenderer {

	DX11Shader::DX11Shader()
	{

	}

	bool DX11Shader::Create(const char* VertexShaderCode, const char* PixelShaderCode, const char* GeometryShaderCode, ShaderType Input)
	{
		ID3D10Blob* ERRMSG;
		bool result = true;
		ERRMSG = 0;

		if (Input == ShaderType::HLSL)
		{
			if (VertexShaderCode != nullptr)
			{
				if (FAILED(D3DCompile(VertexShaderCode, lstrlenA(VertexShaderCode) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", 0, 0, m_VSBL.GetAddressOf(), &ERRMSG)))
				{
					Log->Info("[DX11Shader] Compiling Error -- In Vertex Shader.\nInfo: ");
					CheckShaderErrors(ERRMSG);
					result = false;
				}
			}
			if (PixelShaderCode != nullptr)
			{
				if (FAILED(D3DCompile(PixelShaderCode, lstrlenA(PixelShaderCode) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", 0, 0, m_PSBL.GetAddressOf(), &ERRMSG)))
				{
					Log->Info("[DX11Shader] Compiling Error -- In Pixel Shader.\nInfo: ");
					CheckShaderErrors(ERRMSG);

					result = false;
				}
			}
			if (GeometryShaderCode != nullptr)
			{
				if (FAILED(D3DCompile(GeometryShaderCode, lstrlenA(GeometryShaderCode) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_0", 0, 0, m_GSBL.GetAddressOf(), &ERRMSG)))
				{
					Log->Info("[DX11Shader] Compiling Error -- In Geometry Shader.\nInfo: ");
					CheckShaderErrors(ERRMSG);

					result = false;
				}
			}
		}
		else if (Input == ShaderType::DXBC)
		{
			//Continue loading...
		}
		else {
			Log->Error("[DX11Shader] DirectX 11 Renderer Backend expects all shaders in HLSL or DXBC language!\n");
			return false;
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
			return true;
		else
			return false;
	}

	void DX11Shader::SetUniformBuffer(NRBUniformBuffer * ubuffer)
	{
	}

	void DX11Shader::Delete()
	{
		m_vertexShader.Reset();
		m_pixelShader.Reset();
		m_geometryShader.Reset();
		m_VSBL.Reset();
		m_PSBL.Reset();
		m_GSBL.Reset();
	}

	//inline ID3D11VertexShader * DX11Shader::GetVertexShader()
	//{
	//	return m_vertexShader.Get();
	//}

	//inline ID3D11PixelShader * DX11Shader::GetPixelShader()
	//{
	//	return m_pixelShader.Get();
	//}

	//inline ID3D10Blob * DX11Shader::GetVertexShaderBlob()
	//{
	//	return m_VSBL.Get();
	//}

	//inline ID3D10Blob * DX11Shader::GetPixelShaderBlob()
	//{
	//	return m_PSBL.Get();
	//}

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
			return m_VSBL->GetBufferPointer();
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

		Log->Info(&compileErrors[1]);

		// Release the error message.
		Shader->Release();
		Shader = 0;

		return;
	}
}