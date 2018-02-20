#pragma once
#include <API\Texture.h>
#include <API\Color.h>
#include <API\Shader.h>
#include <unordered_map>

namespace NuclearEngine {
	namespace Shading {

		class NEAPI Material {
		public:

			void SetBool(std::string name, bool value);
			void SetInt(std::string name, int value);
			void SetFloat(std::string name, float value);
			void SetTexture(std::string name, API::Texture* value, unsigned int slot = 0);
			/*void SetVector(std::string name, math::vec2 value);
			void SetVector(std::string name, math::vec3 value);
			void SetVector(std::string name, math::vec4 value);
			void SetMatrix(std::string name, math::mat2 value);
			void SetMatrix(std::string name, math::mat3 value);
			void SetMatrix(std::string name, math::mat4 value);*/
		private:
			API::Shader *m_Shader;
//			std::unordered_map<std::string, UniformValue>        m_Uniforms;
//			std::unordered_map<std::string, UniformValueSampler> m_SamplerUniforms;
		};
	}
}