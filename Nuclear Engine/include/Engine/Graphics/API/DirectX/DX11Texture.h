#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/Texture_Types.h>
#include <array>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				class DX11Shader;
				class DX11VertexShader;
				class DX11PixelShader;
				class DX11RenderTarget;
				class NEAPI DX11Texture
				{
					friend class DX11RenderTarget;
				public:
					DX11Texture();
					~DX11Texture();

					static void Create(DX11Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
					static void Create(DX11Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
					static void Delete(DX11Texture* texture);

					void VSBind(unsigned int slot);
					void GSBind(unsigned int slot);
					void PSBind(unsigned int slot);

					Texture_Desc GetTextureDesc();

				protected:
					Texture_Desc desc;

					static void Create1D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
					static void Create2D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
					static void Create3D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
					static void CreateCube(DX11Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);

					ID3D11Texture1D* tex1D;
					ID3D11Texture2D* tex2D;
					ID3D11Texture3D* tex3D;
					ID3D11ShaderResourceView* resourceView;
				};
			}
		}
	}
}

#endif