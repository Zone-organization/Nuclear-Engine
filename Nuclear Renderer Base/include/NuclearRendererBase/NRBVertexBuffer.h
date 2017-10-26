#pragma once


namespace NuclearRenderer {
	class NRBInputLayout;
	class NRBShader;

	class NRBVertexBuffer
	{
	public:
		/*
		NOTE:

		BufferUsage {

		Default = 0,
		Static = 1,
		Dynamic = 2
		};


		BufferAccess {

		Default = 0,
		ReadOnly = 1,
		WriteOnly = 2,
		};

		*/

		virtual void Create(const void* data, unsigned int size, int usage, int access) = 0;
		virtual void Update(void* data, unsigned int size) = 0;
		virtual void SetInputLayout(NRBInputLayout *layout, NRBShader *shader) = 0;
		virtual void Delete() = 0;
		virtual void Bind() = 0;
		virtual void Unbind()  = 0;
	};
}