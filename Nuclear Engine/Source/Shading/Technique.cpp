#include <Shading\Technique.h>

namespace NuclearEngine {
	namespace Shading {
		Technique::Technique()
		{

		}
		Technique_Type Technique::GetType()
		{
			return this->m_type;
		}

		std::string Technique::GetShaderPath()
		{
			return m_shaderpath;
		}
		std::vector<std::string> Technique::GetDefines()
		{
			return defines;
		}
	}
}