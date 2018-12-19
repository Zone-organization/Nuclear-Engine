#pragma once
#include <Base\NE_Common.h>
#include <vector>
#include <Engine/Graphics/Shader.h>
#include <Core/Path.h>
#include <LLGL/ShaderFlags.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		class NEAPI ShaderCompiler
		{
		public:
			static bool CompileAndCreateShader(Graphics::Shader* result,const std::string& SourceCode, const LLGL::ShaderType& type);
		};
	}
}