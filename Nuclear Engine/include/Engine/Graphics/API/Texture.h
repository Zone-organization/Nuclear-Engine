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
			class NEAPI Texture //<OpenGL::GLTexture, DirectX::DX11Texture>
			{
				friend class RenderTarget;
			public:
				Texture();
				~Texture();

				static void Create(Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static void Create(Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
				static void Delete(Texture* texture);

				Math::Vector2ui GetDimensions();
				unsigned int GetWidth();
				unsigned int GetHeight();
				Uint32 GetHashedName();
				Uint8 GetUsageType();

				void SetHashedName(Uint32 name);
				void SetName(const std::string& str);
				void SetUsageType(Uint8 type);
				void SetUsageType(TextureUsageType type);

				//TODO: Combine Sampler binding methods with these to ensure error-free bindings!
				void VSBind(unsigned int slot);
				void GSBind(unsigned int slot);
				void PSBind(unsigned int slot);

				BASE_API(Texture)
			private:
				Math::Vector2ui mDimensions;
				Uint32 mHashedName;
				Uint8 mUsageType;
			};
		}
	}
}