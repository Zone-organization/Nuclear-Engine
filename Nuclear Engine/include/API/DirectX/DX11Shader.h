#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11VertexBuffer.h>
#include <API\Shader_Types.h>

namespace NuclearEngine
{
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
					BinaryShaderBlob* VertexShaderCode,
					BinaryShaderBlob* PixelShaderCode,
					BinaryShaderBlob* GeometryShaderCode = nullptr);

				void SetConstantBuffer(DX11ConstantBuffer* ubuffer,API::ShaderType type);

				void Bind();
			private:
				unsigned int GetConstantBufferSlot(DX11ConstantBuffer * ubuffer,API::ShaderType type);

				ID3D11VertexShader* VertexShader;
				ID3D11PixelShader* PixelShader;
				ID3D11GeometryShader* GeometryShader;
				
				DXBC_BLOB VSBLOB;
				DXBC_BLOB PSBLOB;
				DXBC_BLOB GSBLOB;
			};
		}
	}
}

#endif