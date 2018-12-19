#pragma once
#include <Engine/Graphics/ShaderCompiler.h>
#ifdef NE_COMPILE_D3DCOMPILER

namespace NuclearEngine 
{
	namespace Graphics
	{
		namespace DXBC_Compiler
		{
			bool CompileHLSL2DXBC_ThenCreate(Graphics::Shader *result,const std::string& SourceCode, LLGL::ShaderType type);

		}
	}
}
#endif
