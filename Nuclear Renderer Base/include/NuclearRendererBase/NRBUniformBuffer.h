#pragma once

namespace NuclearEngine {
	enum class ShaderType;
}

namespace NuclearRenderer {

	/*  Also Known as Constant Buffer in HLSL shader but we call it uniform buffer to avoid confusion 
		since it is constant only in runtime but can be changed at compile time which is very confusing.
	*/
	class NRBUniformBuffer
	{
	public:
		virtual void Create(const char *Nameinshader, unsigned int size, unsigned int Bindingindex) = 0;
		virtual void Update(void* data, unsigned int size, unsigned int offset, unsigned int slot, NuclearEngine::ShaderType type) = 0;
		virtual void Update(void* data, unsigned int size, unsigned int slot, NuclearEngine::ShaderType type) = 0;

		virtual void Delete() = 0;
		virtual unsigned int GetBindingIndex() = 0;
		virtual const char * GetName() = 0;
	protected:
		const char *name;
		unsigned int m_Bindingindex;
	};
}