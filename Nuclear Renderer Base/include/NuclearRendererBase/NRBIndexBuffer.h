#pragma once


namespace NuclearRenderer {
	class NRBIndexBuffer
	{
	public:
		virtual void Create(void* indices, unsigned int count) = 0;
		virtual void Delete() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}