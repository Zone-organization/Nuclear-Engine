#pragma once
#include <API\Texture.h>
#include <API\Color.h>
#include <unordered_map>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Shading {

		class NEAPI Material {
		public:

			//void SetShader(API::Shader *shader);
			void SetBool(std::string name, bool value);
			void SetInt(std::string name, int value);
			void SetFloat(std::string name, float value);
			void SetTexture(std::string name, API::Texture* value, unsigned int slot = 0);
			void SetVector(std::string name, Math::Vector2 value);
			void SetVector(std::string name, Math::Vector3 value);
			void SetVector(std::string name, Math::Vector4 value);
			void SetMatrix(std::string name, Math::Matrix2 value);
			void SetMatrix(std::string name, Math::Matrix3 value);
			void SetMatrix(std::string name, Math::Matrix4 value);
		private:
			API::Shader *m_Shader;
//			std::unordered_map<std::string, UniformValue>        m_Uniforms;
			API::Reflected_Constantbuffer material_constbuf;
		};
	}
}