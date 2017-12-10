#include "..\..\include\Shading\Material.h"

namespace NuclearEngine {
	namespace Shading {
		Material::Material()
		{
		}
		Material::Material(API::Shader * shader, ShaderType type)
		{
			this->SetShader(shader, type);
		}
		API::Shader * Material::GetShader()
		{
			return this->m_Shader;
		}
		void Material::SetShader(API::Shader * shader, ShaderType type)
		{
			m_Shader = shader;
			if (m_Shader != nullptr)
			{
				ShaderReflection reflection = shader->GetReflection(type);

				for (size_t i = 0; i < reflection.textures.size(); i++)
				{
					Material_Textures[reflection.textures.at(i).Name].Name = reflection.textures.at(i).Name.c_str();
				}
			}
		}
		void Material::SetTexture(const char * name, API::Texture * value, unsigned int slot)
		{
			Material_Textures[name].Name = name;
			Material_Textures[name].Slot = slot;
			Material_Textures[name].Texture = value;
		}
	
	}
}