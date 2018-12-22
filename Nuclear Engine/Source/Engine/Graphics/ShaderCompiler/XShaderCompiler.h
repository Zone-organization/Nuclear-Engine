#pragma once

#include <Engine/Graphics/ShaderCompiler.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <Engine/Graphics/Shader.h>

namespace Xsc {
	namespace Reflection {
		struct Field;
		struct ReflectionData;
	}
}
namespace NuclearEngine
{
	namespace Graphics
	{

		class XShaderCompiler
		{
		public:
			static bool Compile(Graphics::Shader *result, const std::string& SourceCode, LLGL::ShaderType type);
			static void Reflect(Xsc::Reflection::ReflectionData* reflection, Graphics::Shader * result);

		private:
			static std::unordered_map<std::string, ShaderVariable> ReflectFields(const std::vector<Xsc::Reflection::Field>& fields);
			static ShaderVariableType Reflect_FieldType(const Xsc::Reflection::Field& field);
		};
	}
}
#endif