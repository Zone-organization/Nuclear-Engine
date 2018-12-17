#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine/Graphics/ShaderTypes.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {
	namespace Graphics {
		BinaryShaderBlob ShaderCompiler::CompileShader(const ShaderLoadDesc& desc, const LLGL::ShaderType& type)
		{
			return CompileShader(Core::FileSystem::LoadShader(desc.Path, desc.Defines, desc.Includes, desc.ReverseIncludeOrder), type);
		}
		bool ShaderCompiler::CompileShader(BinaryShaderBlob* blob, std::string SourceCode, const LLGL::ShaderType& type)
		{
			const auto& languages = Context::GetRenderer()->GetRenderingCaps().shadingLanguages;

			if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::GLSL) != languages.end())
			{
				XShaderCompiler::CompileHLSL2GLSL(blob, SourceCode, type);
			}
			else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::SPIRV) != languages.end())
			{
				return false;
			}
			else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::HLSL) != languages.end())
			{
				DXBC_Compiler::CompileHLSL2DXBC(blob, SourceCode, type);
			}

			blob->isValid = true;
			return true;
		}

		BinaryShaderBlob ShaderCompiler::CompileShader(std::string SourceCode, const LLGL::ShaderType& type)
		{
			BinaryShaderBlob result;
			CompileShader(&result, SourceCode, type);
			return result;
		}


	}
}