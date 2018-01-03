#pragma once
#include <NE_Common.h>
#include <string>
namespace NuclearEngine 
{	
	namespace API 
	{
		struct BinaryShaderBlob;
		enum class ShaderType;
		enum class ShaderLanguage;

		NEAPI BinaryShaderBlob CompileShader(std::string SourceCode,API::ShaderType type,API::ShaderLanguage language);

	}
}