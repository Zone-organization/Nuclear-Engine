#pragma once

namespace NuclearEngine {
	enum class ShaderType;
}

struct ID3D11Buffer;

namespace NuclearRenderer {

	/*  Also Known as Constant Buffer in HLSL shader but we call it uniform buffer to avoid confusion 
		since it is constant only in runtime but can be changed at compile time which is very confusing.
	*/
	class NRBUniformBuffer
	{
	public:
		virtual void Create(const char *Nameinshader, unsigned int size) = 0;
		virtual void Update(const void* data, unsigned int size) = 0;

		virtual void Delete() = 0;
		virtual unsigned int GetBindingIndex() = 0;
		virtual const char * GetName() = 0;

		//Directx 11 only
		virtual ID3D11Buffer* const *GetDXBuffer() = 0;

		//Opengl
		unsigned int shaderidx;
	protected:
		const char *name;
		unsigned int m_Bindingindex;
	};
}