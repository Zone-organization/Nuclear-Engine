#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
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
			class NEAPI Texture
			{
				friend class RenderTarget;
			public:
				Texture();
				~Texture();

				static void Create(Texture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static void Create(Texture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
				static void Delete(Texture* texture);

				void VSBind(unsigned int slot);
				void GSBind(unsigned int slot);
				void PSBind(unsigned int slot);

			private:
				OpenGL::GLTexture GLObject;
				DirectX::DX11Texture DXObject;
			};
		}
	}
}

#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLTexture.h>
namespace NuclearEngine
{
	namespace Graphics 
	{
		namespace API
		{
			typedef OpenGL::GLTexture Texture;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11Texture.h>
namespace NuclearEngine
{
	namespace Graphics {

		namespace API
		{
			typedef DirectX::DX11Texture Texture;
		}
	}
}
#endif
#endif