#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLTexture.h>
#include <Engine/Graphics/API/DirectX\DX11Texture.h>
#include <Base\Math\Math.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		enum TextureUsageType : Uint8
		{
			Diffuse = 1,
			Specular = 2,
			Normal = 3
		};
		namespace API
		{
			class VertexShader;
			class PixelShader;
			class RenderTarget;
			class NEAPI Texture
			{
				friend class RenderTarget;
			public:
				Texture();
				~Texture();

				static bool Create(Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static bool Create(Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
				static void Delete(Texture* texture);

				Math::Vector3ui GetDimensions(Uint8 Mipmaplevel = 0);
				Texture_Desc GetTextureDesc();

				//TODO: Combine Sampler binding methods with these to ensure error-free bindings!
				void VSBind(Uint8 slot);
				void GSBind(Uint8 slot);
				void PSBind(Uint8 slot);

				BASE_API(Texture)

			protected:
				Texture_Desc mTexDesc;
			};
		}
	}
}