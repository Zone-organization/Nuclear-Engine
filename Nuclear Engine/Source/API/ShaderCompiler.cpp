#define NOMINMAX

#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {

	namespace API {

	
		bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode, API::ShaderType type)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				XShaderCompiler::CompileHLSL2GLSL(blob, SourceCode, type);
			}
			else
			{
				DXBC_Compiler::CompileHLSL2DXBC(blob, SourceCode, type);
			}

			blob->Finished = true;
			return true;
		}

		BinaryShaderBlob CompileShader(std::string SourceCode, API::ShaderType type)
		{
			BinaryShaderBlob result;
			CompileShader(&result, SourceCode, type);
			return result;
		}

	}
}