#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBShader.h>

namespace NuclearRenderer {

	class NRDX11API DX11Shader : public NRBShader
	{
	public:
		DX11Shader();

		bool Create(const char* VertexShaderCode, const char* PixelShaderCode, const char* GeometryShaderCode, NuclearEngine::ShaderLanguage Input) override;
		void SetUniformBuffer(NRBUniformBuffer* ubuffer) override;
		unsigned int GetUniformBufferSlot(NRBUniformBuffer* ubuffer, NuclearEngine::ShaderType type) override;

		void Bind() override;
		void Unbind() override;
		void Delete() override;

		//OpenGL Specific
		unsigned int GetGLShaderID() override;   //Note: Do Nothing here!!

		//DirectX 11 Specific 
		void * GetDXBufferPointer() override;
		unsigned long GetDXBufferSize() override;

	private:

		void CheckShaderErrors(ID3D10Blob* Shader);

		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11PixelShader> m_pixelShader;
		ComPtr<ID3D11GeometryShader> m_geometryShader;
		
		ComPtr<ID3D10Blob> m_VSBL;
		ComPtr<ID3D10Blob> m_PSBL;
		ComPtr<ID3D10Blob> m_GSBL;
	};
}