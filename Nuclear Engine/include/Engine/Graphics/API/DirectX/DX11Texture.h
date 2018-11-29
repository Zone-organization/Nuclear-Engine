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

					static bool Create(DX11Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
					static bool Create(DX11Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
					static void Delete(DX11Texture* texture);

					void VSBind(Uint8 slot);
					void GSBind(Uint8 slot);
					void PSBind(Uint8 slot);

					Math::Vector3ui GetDimensions(Uint8 Mipmaplevel);

					ID3D11ShaderResourceView* mResourceView;

				protected:
					static bool Create1D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
					static bool Create2D(DX11Texture* texture, const Texture_Data& Data, const Texture_Desc& Desc);
				};
			}
		}
	}
}

#endif