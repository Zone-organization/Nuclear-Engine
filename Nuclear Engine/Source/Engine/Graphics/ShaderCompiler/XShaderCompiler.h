#pragma once

#include <Engine/Graphics/ShaderCompiler.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <Engine/Graphics/Shader.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace XShaderCompiler
		{
			bool CompileHLSL2GLSL_ThenCreate(Graphics::Shader *result, const std::string& SourceCode, LLGL::ShaderType type);
		}
	}
}
#endif