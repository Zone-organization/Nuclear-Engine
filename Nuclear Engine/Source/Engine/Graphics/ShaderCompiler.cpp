#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine/Graphics/ShaderTypes.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			NEAPI BinaryShaderBlob CompileShader(const ShaderLoadDesc& desc, const LLGL::ShaderType& type)
			{
				return CompileShader(Core::FileSystem::LoadShader(desc.Path, desc.Defines, desc.Includes, desc.ReverseIncludeOrder), type);
			}
			bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode, const LLGL::ShaderType& type)
			{
				if (Graphics::Context::IsOpenGL())
				{
					XShaderCompiler::CompileHLSL2GLSL(blob, SourceCode, type);
				}
				else
				{
					DXBC_Compiler::CompileHLSL2DXBC(blob, SourceCode, type);
				}

				blob->isValid = true;
				return true;
			}

			BinaryShaderBlob CompileShader(std::string SourceCode, const LLGL::ShaderType& type)
			{
				BinaryShaderBlob result;
				CompileShader(&result, SourceCode, type);
				return result;
			}

		}
	}
}