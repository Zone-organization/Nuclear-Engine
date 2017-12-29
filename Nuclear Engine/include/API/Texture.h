#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDERER
#include <API\OpenGL\GLTexture.h>
#include <API\DirectX\DX11Texture.h>

namespace NuclearEngine {
	namespace API {
		class Shader;
		class NEAPI Texture
		{
		public:
			Texture();			
			~Texture();
			
			static void Create(Texture* texture, Texture_Data* TexData, Texture_Desc* Desc);
			static void Create(Texture* texture, const std::array<NuclearEngine::Texture_Data*, 6>& data, NuclearEngine::Texture_Desc* Desc);

			void VSBind(unsigned int index);
			void VSBind(const char *samplerName, Shader *shader, unsigned int index);
			void GSBind(unsigned int index);
			void GSBind(const char *samplerName, Shader *shader, unsigned int index);
			void PSBind(unsigned int index);
			void PSBind(const char *samplerName, Shader *shader, unsigned int index);

		private:
		private:
			OpenGL::GLTexture GLObject;
			DirectX::DX11Texture DXObject;
		};
	}
}

#else
#ifdef NE_USE_OPENGL3_3
#include <API\OpenGL\GLTexture.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLTexture Texture;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11Texture.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11Texture Texture;
	}
}
#endif
#endif