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

				class NEAPI DX11PixelShader
				{
				public:
					DX11PixelShader();
					~DX11PixelShader();

					static void Create(DX11PixelShader *shader, const BinaryShaderBlob& desc);
					static void Delete(DX11PixelShader *shader);

					void SetConstantBuffer(DX11ConstantBuffer* ubuffer);

					void Bind();

					ID3D11PixelShader * mPixelShader;
				};
			}
		}
	}
}

#endif
