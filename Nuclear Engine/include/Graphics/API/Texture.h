#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLTexture.h>
#include <Graphics/API/DirectX\DX11Texture.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			class VertexShader;
			class PixelShader;
			class RenderTarget;
			class NEAPI Texture : public CBaseAPI<OpenGL::GLTexture, DirectX::DX11Texture>
			{
				friend class RenderTarget;
			public:
				Texture();
				~Texture();

				static void Create(Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static void Create(Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
				static void Delete(Texture* texture);


				//TODO: Combine Sampler binding methods with these to ensure error-free bindings!
				void VSBind(unsigned int slot);
				void GSBind(unsigned int slot);
				void PSBind(unsigned int slot);

			};
		}
	}
}