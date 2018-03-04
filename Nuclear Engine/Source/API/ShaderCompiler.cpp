#define NOMINMAX

#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {

	namespace API {

	
		bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode,API::ShaderType type,API::ShaderLanguage language)
		{
			if (language == API::ShaderLanguage::HLSL)
			{
				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					XShaderCompiler::CompileHLSL2GLSL(blob, SourceCode, type, language);
				}
				else
				{
					DXBC_Compiler::CompileHLSL2DXBC(blob, SourceCode, type, language);
				}
			}
			else if (language == API::ShaderLanguage::GLSL)
			{
				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					blob->GLSL_SourceCode = SourceCode;
					blob->Language = API::ShaderLanguage::GLSL;
				}
				else 
				{
					Exceptions::NotImplementedException();
				}
			}
			blob->Finished = true;
			return true;
		}

		BinaryShaderBlob CompileShader(std::string SourceCode, API::ShaderType type, API::ShaderLanguage language)
		{
			BinaryShaderBlob result;
			CompileShader(&result, SourceCode, type,language);
			return result;
		}

	}
}