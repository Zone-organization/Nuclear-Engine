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

			class NEAPI DX11PixelShader
			{
			public:
				DX11PixelShader();
				~DX11PixelShader();

				static void Create(DX11PixelShader *shader, BinaryShaderBlob* desc);
				static void Delete(DX11PixelShader *shader);

				void SetConstantBuffer(DX11ConstantBuffer* ubuffer, API::ShaderType type);

				void Bind();
			private:
				unsigned int GetConstantBufferSlot(DX11ConstantBuffer * ubuffer, API::ShaderType type);

				ID3D11PixelShader* PixelShader;
			};
		}
	}
}

#endif
