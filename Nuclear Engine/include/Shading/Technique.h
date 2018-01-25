#pragma once
#include <API\Shader.h>
#include <vector>
namespace NuclearEngine 
{
	namespace Shading
	{

		enum class NEAPI Technique_Type 
		{
			Nothing,
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
			virtual std::vector<std::string> GetDefines();
		protected:
			Technique_Type m_type = Technique_Type::Nothing;
			std::string m_shaderpath = "";
			std::vector<std::string> defines;
		};
	}
}