#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/Shader_Types.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
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

					static void Create(DX11VertexShader *shader, const BinaryShaderBlob& desc);
					static void Delete(DX11VertexShader *shader);

					void SetConstantBuffer(DX11ConstantBuffer* ubuffer);

					void Bind();


					ID3D11VertexShader * mVertexShader;
					ShaderReflection Reflection;

					/*Used for InputLayouts*/
					void* VS_Buffer;
					size_t VS_Size;
				};
			}
		}
	}
}

#endif