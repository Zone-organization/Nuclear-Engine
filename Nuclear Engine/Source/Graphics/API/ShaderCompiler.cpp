#include <Graphics/API/ShaderCompiler.h>
#include <Graphics/API/Shader_Types.h>
#include <Graphics\API\Context.h>
#include <Core\FileSystem.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			NEAPI BinaryShaderBlob CompileShader(const ShaderLoadDesc& desc, const Graphics::API::ShaderType& type)
			{
				return CompileShader(Core::FileSystem::LoadShader(desc.Path, desc.Defines, desc.Includes, desc.ReverseIncludeOrder), type);
			}
			bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode, const Graphics::API::ShaderType& type)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
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

			BinaryShaderBlob CompileShader(std::string SourceCode, const Graphics::API::ShaderType& type)
			{
				BinaryShaderBlob result;
				CompileShader(&result, SourceCode, type);
				return result;
			}

		}
	}
}