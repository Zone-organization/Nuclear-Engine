#include <Shading\Technique.h>

namespace NuclearEngine {
	namespace Shading {
		Technique::Technique()
		{

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