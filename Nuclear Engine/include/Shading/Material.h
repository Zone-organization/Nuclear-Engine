#pragma once
#include <API\Texture.h>
#include <API\TextureCube.h>
#include <API\Color.h>
#include <API\Shader.h>
namespace NuclearEngine {
	namespace Shading {

		class Material {
		public:
			Material(API::Shader* shader);

			API::Shader* GetShader();
			void    SetShader(API::Shader* shader);

			API::Color SolidColor;

			void SetTexture(const char* name, API::Texture* value, unsigned int unit = 0);
			void SetTextureCube(const char* name, API::TextureCube* value, unsigned int unit = 0);

		private:
			API::Shader* m_Shader;

		};
	}
}