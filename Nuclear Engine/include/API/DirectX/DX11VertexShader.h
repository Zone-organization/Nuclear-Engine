#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11

namespace NuclearEngine
{
	namespace API
	{
		struct BinaryShaderBlob;
		enum class ShaderType;
		namespace DirectX
		{
			class DX11ConstantBuffer;
			class DX11VertexBuffer;

			class NEAPI DX11VertexShader
			{
				friend class DX11VertexBuffer;
			public:
				DX11VertexShader();
				~DX11VertexShader();

				static void Create(DX11VertexShader *shader, BinaryShaderBlob* desc);
				static void Delete(DX11VertexShader *shader);

				void SetConstantBuffer(DX11ConstantBuffer* ubuffer, API::ShaderType type);

				void Bind();
			private:
				unsigned int GetConstantBufferSlot(DX11ConstantBuffer * ubuffer, API::ShaderType type);

				ID3D11VertexShader* VertexShader;

				/*Used for InputLayouts*/
				void* VS_Buffer;
				size_t VS_Size;
			};
		}
	}
}

#endif