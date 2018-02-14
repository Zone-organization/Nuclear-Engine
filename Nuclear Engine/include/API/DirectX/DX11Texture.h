#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\Texture_Types.h>
#include <array>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class DX11Shader;
			class DX11VertexShader;
			class DX11PixelShader;

			class NEAPI DX11Texture
			{
			public:
				DX11Texture();
				~DX11Texture();

				static void Create(DX11Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static void Create(DX11Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
				static void Delete(DX11Texture* texture);

				void SetInShader(const char *samplerName, DX11VertexShader *shader, unsigned int index);
				void SetInShader(const char *samplerName, DX11PixelShader *shader, unsigned int index);

				void VSBind(unsigned int index);
				void VSBind(const char *samplerName, DX11Shader *shader, unsigned int index);
				void GSBind(unsigned int index);
				void GSBind(const char *samplerName, DX11Shader *shader, unsigned int index);
				void PSBind(unsigned int index);
				void PSBind(const char *samplerName, DX11Shader *shader, unsigned int index);

			protected:
				static void Create1D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
				static void Create2D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
				static void Create3D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
				static void CreateCube(DX11Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);

				ID3D11Texture1D* tex1D;
				ID3D11Texture2D* tex2D;
				ID3D11Texture3D* tex3D;
				ID3D11ShaderResourceView* resourceView;
				ID3D11SamplerState* samplerState;
			};
		}
	}
}

#endif