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

				class NEAPI DX11UnifiedShader
				{
					friend class DX11VertexBuffer;
				public:
					DX11UnifiedShader();
					~DX11UnifiedShader();

					static void Create(DX11UnifiedShader *shader, const BinaryShaderBlob& VS_Desc, const BinaryShaderBlob& PS_Desc);
					static void Delete(DX11UnifiedShader *shader);

					void SetConstantBuffer(DX11ConstantBuffer* ubuffer,ShaderReflection& reflection,const ShaderType& type);

					void Bind();
				private:

					ID3D11VertexShader * VertexShader;
					ID3D11PixelShader * PixelShader;

					/*Used for InputLayouts*/
					void* VS_Buffer;
					size_t VS_Size;
				};
			}
		}
	}
}

#endif