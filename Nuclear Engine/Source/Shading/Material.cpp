#include "..\..\include\Shading\Material.h"

namespace NuclearEngine {
	namespace Shading {
		void Material::SetShader(API::Shader * shader)
		{
			m_Shader = shader;

		}
		void Material::SetBool(std::string name, bool value)
		{
		}
		void Material::SetInt(std::string name, int value)
		{
		}
		void Material::SetFloat(std::string name, float value)
		{
		}
		void Material::SetTexture(std::string name, API::Texture * value, unsigned int slot)
		{
		}
		void Material::SetVector(std::string name, Math::Vector2 value)
		{
		}
		void Material::SetVector(std::string name, Math::Vector3 value)
		{
		}
		void Material::SetVector(std::string name, Math::Vector4 value)
		{
		}
		void Material::SetMatrix(std::string name, Math::Matrix2 value)
		{
		}
		void Material::SetMatrix(std::string name, Math::Matrix3 value)
		{
		}
		void Material::SetMatrix(std::string name, Math::Matrix4 value)
		{
		}
	}
}