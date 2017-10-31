#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBShader.h>

namespace NuclearRenderer
{
	class NROGL3API GLShader : public NuclearRenderer::NRBShader
	{
	public:
		GLShader();

		bool Create(const char* VertexShaderCode, const char* PixelShaderCode, const char* GeometryShaderCode, NuclearEngine::ShaderLanguage Input) override;
		void SetUniformBuffer(NRBUniformBuffer* ubuffer, unsigned int slot, NuclearEngine::ShaderType type) override;
		unsigned int GetUniformBufferSlot(NRBUniformBuffer* ubuffer, NuclearEngine::ShaderType type) override;

		void Bind() override;
		void Unbind() override;
		void Delete() override;

		//OpenGL Specific
		unsigned int GetGLShaderID() override;

		//DirectX 11 Specific  Note: Do Nothing here!!
		void * GetDXBufferPointer() override;
		unsigned long GetDXBufferSize() override;

	private:
		GLuint _ProgramID;     //<-- Handler Of The Shader Be careful with it
	};
}