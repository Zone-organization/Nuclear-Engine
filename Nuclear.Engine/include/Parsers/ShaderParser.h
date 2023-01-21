#pragma once
#include <Assets/Shader.h>

namespace Nuclear
{
	namespace Parsers
	{
		class NEAPI ShaderParser
		{
		public:
			static bool ParseSource(const std::string& source, Graphics::ShaderBuildDesc& desc);

		};
	}
}