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
		std::string Technique::GetShader()
		{
			return m_shader;
		}
	}
}