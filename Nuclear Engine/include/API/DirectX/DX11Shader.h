#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11VertexBuffer.h>
namespace NuclearEngine
{
	struct BinaryShaderBlob;
	enum class ShaderType;

	namespace API
	{
		namespace DirectX
		{
			class DX11ConstantBuffer;


			class NEAPI DX11Shader
			{
				friend class DX11VertexBuffer;
			public:
				DX11Shader();
				~DX11Shader();

				static void Create(DX11Shader *shader,
					NuclearEngine::BinaryShaderBlob* VertexShaderCode,
					NuclearEngine::BinaryShaderBlob* PixelShaderCode,
					NuclearEngine::BinaryShaderBlob* GeometryShaderCode = nullptr);

				void SetConstantBuffer(DX11ConstantBuffer* ubuffer, ShaderType type);

				void Bind();
				void Unbind();

			private:

				void CheckShaderErrors(ID3D10Blob* Shader);
				unsigned int GetConstantBufferSlot(DX11ConstantBuffer * ubuffer, ShaderType type);

				ID3D11VertexShader* m_vertexShader;
				ID3D11PixelShader* m_pixelShader;
				ID3D11GeometryShader* m_geometryShader;

				NuclearEngine::BinaryShaderBlob* m_VSBL;
				NuclearEngine::BinaryShaderBlob* m_PSBL;
				NuclearEngine::BinaryShaderBlob* m_GSBL;
			};
		}
	}
}

#endif