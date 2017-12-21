#pragma once
#include <NuclearRendererBase\NRBConstantBuffer.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer
{
	class NROGL3API GLConstantBuffer : public NRBConstantBuffer
	{
	public:
		GLConstantBuffer();

		void Create(const char *Nameinshader, unsigned int size) override;
		void Update(const void* data, unsigned int size) override;
		void Delete() override;
		unsigned int GetBindingIndex() override;
		const char * GetName() override;

		ID3D11Buffer* const *GetDXBuffer();
	protected:
		GLuint buffer;
	};
}