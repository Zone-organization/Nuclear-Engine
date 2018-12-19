#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine/Graphics/ShaderTypes.h>
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include "ShaderCompiler\DXBC_Compiler.h"
#include "ShaderCompiler\XShaderCompiler.h"

namespace NuclearEngine {
	namespace Graphics {

		bool ShaderCompiler::CompileAndCreateShader(Graphics::Shader* result,const std::string& SourceCode, const LLGL::ShaderType& type)
		{
			const auto& languages = Context::GetRenderer()->GetRenderingCaps().shadingLanguages;

			if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::GLSL) != languages.end())
			{
				XShaderCompiler::CompileHLSL2GLSL_ThenCreate(result, SourceCode, type);
			}
			else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::SPIRV) != languages.end())
			{
				return false;
			}
			else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::HLSL) != languages.end())
			{
				DXBC_Compiler::CompileHLSL2DXBC_ThenCreate(result, SourceCode, type);
			}

			return true;
		}
	}
}