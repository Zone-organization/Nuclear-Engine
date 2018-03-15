#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>
#include <Core\FileSystem.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {

	namespace API {
		NEAPI BinaryShaderBlob CompileShader(const ShaderLoadDesc& desc, const API::ShaderType& type)
		{
			return CompileShader(Core::FileSystem::LoadShader(desc.Path, desc.Defines, desc.Includes, desc.ReverseIncludeOrder), type);
		}
		bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode, const API::ShaderType& type)
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

		BinaryShaderBlob CompileShader(std::string SourceCode, const API::ShaderType& type)
		{
			BinaryShaderBlob result;
			CompileShader(&result, SourceCode, type);
			return result;
		}

	}
}