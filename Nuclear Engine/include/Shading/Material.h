#pragma once
#include <API\Texture.h>
#include <API\Color.h>
#include <API\Shader.h>
#include <map>

namespace NuclearEngine {
	namespace Shading {
		struct Material_Texture
		{
			const char* Name;
			unsigned int Slot;			
			API::Texture *Texture;			
		};

		class Material {
		public:
			std::map<std::string, Material_Texture> Material_Textures; // NOTE(Joey): process samplers differently 

			Material();
			Material(API::Shader* shader);

			API::Shader* GetShader();
			void    SetShader(API::Shader* shader);

			API::Color SolidColor;

			void SetTexture(const char* name, API::Texture* value, unsigned int slot = 0);

		private:
			API::Shader* m_Shader;

		};
	}
}