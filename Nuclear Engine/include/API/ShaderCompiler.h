#pragma once
#include <NE_Common.h>

namespace NuclearEngine {

	struct BinaryShaderBlob;
	enum class ShaderType;
	enum class ShaderLanguage;

	namespace API {

		NEAPI BinaryShaderBlob CompileShader(const char* SourceCode, ShaderType type, ShaderLanguage language);

	}
}