#pragma once

#include <Engine/Graphics/ShaderCompiler.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <Engine/Graphics/ShaderTypes.h>

namespace NuclearEngine
{
	namespace Graphics 
	{

		namespace API
		{
			namespace XShaderCompiler
			{
				void CompileHLSL2GLSL(BinaryShaderBlob *result, std::string SourceCode, LLGL::ShaderType type);
			}
		}
	}
}
#endif