#pragma once
#include <NuclearRendererBase\NRBUniformBuffer.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer
{
	class NROGL3API GLUniformBuffer : public NRBUniformBuffer
	{
	public:
		GLUniformBuffer();

		void Create(const char *Nameinshader, unsigned int size, unsigned int Bindingindex) override;
		void Update(void* data, unsigned int size, unsigned int offset, unsigned int slot, NuclearEngine::ShaderType type) override;
		void Update(void* data, unsigned int size, unsigned int slot, NuclearEngine::ShaderType type) override;
		void Delete() override;
		unsigned int GetBindingIndex() override;
		const char * GetName() override;

	protected:
		GLuint buffer;
	};
}