#pragma once
#include <API\ShaderCompiler.h>
#ifdef NE_COMPILE_D3DCOMPILER

namespace NuclearEngine 
{
	namespace API
	{
		namespace DXBC_Compiler
		{
			void CompileHLSL2DXBC(BinaryShaderBlob *result, std::string SourceCode, API::ShaderType type);

		}
	}
}
#endif
