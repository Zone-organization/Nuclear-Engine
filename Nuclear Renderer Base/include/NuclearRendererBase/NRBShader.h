#pragma once

namespace NuclearEngine {
	enum class ShaderLanguage;
	enum class ShaderType;
	struct BinaryShaderBlob;
}

namespace NuclearRenderer {
	class NRBUniformBuffer;
	class NRBShader
	{
	public:
		virtual bool Create(NuclearEngine::BinaryShaderBlob* VertexShaderCode, NuclearEngine::BinaryShaderBlob* PixelShaderCode, NuclearEngine::BinaryShaderBlob* GeometryShaderCode) = 0;

		virtual void SetUniformBuffer(NRBUniformBuffer* ubuffer, unsigned int slot, NuclearEngine::ShaderType type) = 0;
		virtual unsigned int GetUniformBufferSlot(NRBUniformBuffer* ubuffer, NuclearEngine::ShaderType type) = 0;

		virtual void Delete() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		//OpenGL Specific
		virtual unsigned int GetGLShaderID() = 0;

		//DirectX 11 Specific
		virtual void * GetDXBufferPointer() = 0;
		virtual unsigned long GetDXBufferSize() =0;
	};
}