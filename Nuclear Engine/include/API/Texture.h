#pragma once
#include <NE_Common.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBTexture;
}

namespace NuclearEngine {
	namespace API {
		class Shader;

		class NEAPI Texture
		{
		public:
			Texture(const Texture_Data& TexData, const Texture_Desc& Desc);
			~Texture();
			
			void VSBind(unsigned int index);
			void VSBind(const char *samplerName, API::Shader *shader, unsigned int index);
			void GSBind(unsigned int index);
			void GSBind(const char *samplerName, API::Shader *shader, unsigned int index);
			void PSBind(unsigned int index);
			void PSBind(const char *samplerName, API::Shader *shader, unsigned int index);

			Texture_Data& GetTextureData() { return Data; }
			NuclearRenderer::NRBTexture *GetBase();
		protected:
			Texture_Data Data;
			NuclearRenderer::NRBTexture *tex;
		};
	}
}