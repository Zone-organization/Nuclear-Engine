#pragma once
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <vector>
#include <Base\NE_Common.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			std::vector<ShaderVariableDesc> ReflectHLSL(const std::string& SourceCode, SHADER_TYPE type);
		}
	}
}
