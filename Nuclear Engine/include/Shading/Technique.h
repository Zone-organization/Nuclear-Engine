#pragma once
#include <API\Shader.h>

namespace NuclearEngine {
	namespace Shading {

		enum class NEAPI Technique_Type 
		{
			// Totally Ignored
			Nothing,
			// Main Entry Point float4 DoLighting(float2 TexCoords);
			LightShading
		};

		struct NEAPI Technique_Requirements
		{
			bool Normals = false;
		};

		// Interface
		class NEAPI Technique {
		public:
			Technique();

			virtual Technique_Type GetType();
			virtual std::string GetShaderPath();

		protected:
			Technique_Type m_type = Technique_Type::Nothing;
			std::string m_shaderpath = "";
		};
	}
}