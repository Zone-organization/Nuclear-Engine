#pragma once
#include <Graphics/API/Texture.h>
#include <Graphics/Color.h>
#include <unordered_map>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Shading {

		class NEAPI Material {
		public:

			//void SetShader(Graphics::API::Shader *shader);
			void SetBool(std::string name, bool value);
			void SetInt(std::string name, int value);
			void SetFloat(std::string name, float value);
			void SetTexture(std::string name, Graphics::API::Texture* value, unsigned int slot = 0);
			void SetVector(std::string name, Math::Vector2 value);
			void SetVector(std::string name, Math::Vector3 value);
			void SetVector(std::string name, Math::Vector4 value);
			void SetMatrix(std::string name, Math::Matrix2 value);
			void SetMatrix(std::string name, Math::Matrix3 value);
			void SetMatrix(std::string name, Math::Matrix4 value);
		private:
			//Graphics::API::Shader *m_Shader;
//			std::unordered_map<std::string, UniformValue>        m_Uniforms;
			//Graphics::API::Reflected_Constantbuffer material_constbuf;
		};
	}
}