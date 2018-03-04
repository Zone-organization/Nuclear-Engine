#pragma once

#include <API\ShaderCompiler.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <API\Shader_Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace XShaderCompiler
		{
			void CompileHLSL2GLSL(BinaryShaderBlob *result, std::string SourceCode, API::ShaderType type);
		}
	}
}
#endif