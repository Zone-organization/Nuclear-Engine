#include "..\..\include\Shading\Material.h"


namespace NuclearEngine {
	namespace Shading {
		Material::Material()
		{
		}
		Material::Material(API::Shader * shader)
		{
		}
		API::Shader * Material::GetShader()
		{
			return nullptr;
		}
		void Material::SetShader(API::Shader * shader)
		{
		}
		void Material::SetTexture(const char * name, API::Texture * value, unsigned int slot)
		{
			Material_Textures[name].Name = name;
			Material_Textures[name].Slot = slot;
			Material_Textures[name].Texture = value;
		}
	
	}
}