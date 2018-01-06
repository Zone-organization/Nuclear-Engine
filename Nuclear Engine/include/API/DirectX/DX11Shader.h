#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11

namespace NuclearEngine
{
	namespace API
	{
		struct ShaderDesc;
		enum class ShaderType;
		namespace DirectX
		{
			class DX11ConstantBuffer;
			class DX11VertexBuffer;

			class NEAPI DX11Shader
			{
				friend class DX11VertexBuffer;
			public:
				DX11Shader();
				~DX11Shader();

				static void Create(DX11Shader *shader, ShaderDesc* desc);

				void SetConstantBuffer(DX11ConstantBuffer* ubuffer,API::ShaderType type);

				void Bind();
			private:
				unsigned int GetConstantBufferSlot(DX11ConstantBuffer * ubuffer,API::ShaderType type);

				ID3D11VertexShader* VertexShader;
				ID3D11PixelShader* PixelShader;
				ID3D11GeometryShader* GeometryShader;
				
				/*Used for InputLayouts*/
				void* VS_Buffer;
				size_t VS_Size;
			};
		}
	}
}

#endif