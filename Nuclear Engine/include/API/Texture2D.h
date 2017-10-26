#pragma once
#include <NE_Common.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBTexture2D;
}

namespace NuclearEngine {
	namespace API {
		class Shader;

		class NEAPI Texture2D
		{
		public:
			Texture2D(const Texture_Data& TexData, const Texture_Desc& Desc);
			~Texture2D();
			
			void Bind(unsigned int index);
			void Bind(const char* samplerName, API::Shader* shader, unsigned int index);
			void Unbind();
			void Unbind(unsigned int index);

			Texture_Data& GetTextureData() { return Data; }
			NuclearRenderer::NRBTexture2D *GetBase();
		protected:
			Texture_Data Data;
			NuclearRenderer::NRBTexture2D *tex;
		};
	}
}