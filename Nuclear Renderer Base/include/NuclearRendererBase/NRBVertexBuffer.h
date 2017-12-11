#pragma once

namespace NuclearEngine {
	enum class BufferGPUUsage;
	enum class BufferCPUAccess;
}

namespace NuclearRenderer {
	class NRBInputLayout;
	class NRBShader;

	class NRBVertexBuffer
	{
	public:
		virtual void Create(const void* data, unsigned int size, NuclearEngine::BufferGPUUsage usage, NuclearEngine::BufferCPUAccess access) = 0;
		virtual void Update(const void* data, unsigned int size) = 0;
		virtual void SetInputLayout(NRBInputLayout *layout, NRBShader *shader) = 0;
		virtual void Delete() = 0;
		virtual void Bind() = 0;
		virtual void Unbind()  = 0;
	};
}