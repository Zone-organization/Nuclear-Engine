#pragma once
#include <Graphics/API/ShaderCompiler.h>
#ifdef NE_COMPILE_D3DCOMPILER

namespace NuclearEngine 
{
	namespace Graphics 
	{
		namespace API
		{
			namespace DXBC_Compiler
			{
				void CompileHLSL2DXBC(BinaryShaderBlob *result, std::string SourceCode, Graphics::API::ShaderType type);

			}
		}
	}
}
#endif
