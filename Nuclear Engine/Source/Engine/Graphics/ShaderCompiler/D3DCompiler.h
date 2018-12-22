#pragma once
#include <Engine/Graphics/ShaderCompiler.h>
#ifdef NE_COMPILE_D3DCOMPILER
#include <d3dcompiler.h>

namespace NuclearEngine 
{
	namespace Graphics
	{
		class D3DCompiler
		{
		public:

			static bool Compile(Graphics::Shader *result,const std::string& SourceCode, LLGL::ShaderType type);
			static void Reflect(Graphics::Shader *result, ID3D10Blob* blob);

		private:
			static inline ShaderVariableType GetShaderVariableType(const D3D11_SHADER_TYPE_DESC & VarTypeDesc);
			static void Check_D3DCompile_Errors(ID3D10Blob* Shader);

			static void ParseStruct(const D3D11_SHADER_VARIABLE_DESC& sdesc, const D3D11_SHADER_TYPE_DESC& tdesc, Graphics::Shader *result);
			static void ParseConstantBuffers(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, Graphics::Shader *result);
			static void ParseResources(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, Graphics::Shader *result);
		};
	}
}
#endif
