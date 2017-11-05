#pragma once
#include <NE_Common.h>
#include <API\UniformBuffer.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBShader;
}
struct ID3D10Blob;

namespace NuclearEngine {
	namespace API {

		class NEAPI Shader
		{
		public:
			Shader(const char *name, const char* VertexShader, const char* PixelShader, const char* GeometryShader, ShaderLanguage inputtype);
			~Shader();

			void SetUniformBuffer(UniformBuffer* cbuffer, unsigned int slot , ShaderType type);
			unsigned int GetUniformBufferSlot(UniformBuffer* cbuffer, ShaderType type);
			void Bind();
			void Unbind();

			NuclearRenderer::NRBShader* GetBase();
		protected:
			NuclearRenderer::NRBShader *shader;

			ID3D10Blob* m_VSBL;
			ID3D10Blob* m_PSBL;
			ID3D10Blob* m_GSBL;
		};
	}
}