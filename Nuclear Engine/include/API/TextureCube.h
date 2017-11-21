#pragma once
#include <NE_Common.h>
#include <array>
#include <NuclearCommon\Common_API_Types.h>
#include <API\Shader.h>

namespace NuclearRenderer
{
	class NRBTextureCube;
}

namespace NuclearEngine {
	namespace API {

		class NEAPI TextureCube
		{
		public:
			TextureCube(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc);
			~TextureCube();

			void Bind(unsigned int index);
			void Bind(const char *samplerName, Shader *shader, unsigned int index);
			void Unbind();
			void Unbind(unsigned int index);

		private:
			NuclearRenderer::NRBTextureCube *tex;
		};
	}
}