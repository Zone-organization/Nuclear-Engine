#pragma once
#include <NE_Common.h>
#include <API\UniformBuffer.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBShader;
}

namespace NuclearEngine {
	namespace API {

		NEAPI BinaryShaderBlob CompileShader(const char* SourceCode, ShaderType type, ShaderLanguage language);

		class NEAPI Shader
		{
		public:
			//Now expects all shaders in HLSL!!
			Shader(const char *name, BinaryShaderBlob* VertexShader, BinaryShaderBlob* PixelShader, BinaryShaderBlob* GeometryShader = nullptr);
			~Shader();

			void SetUniformBuffer(UniformBuffer* cbuffer, unsigned int slot , ShaderType type);
			unsigned int GetUniformBufferSlot(UniformBuffer* cbuffer, ShaderType type);
			void Bind();
			void Unbind();

			ShaderReflection  GetReflection(ShaderType type);

			NuclearRenderer::NRBShader* GetBase();
		protected:
			NuclearRenderer::NRBShader *shader;

			BinaryShaderBlob* vblob;
			BinaryShaderBlob* pblob;
			BinaryShaderBlob* gblob;
		};
	}
}