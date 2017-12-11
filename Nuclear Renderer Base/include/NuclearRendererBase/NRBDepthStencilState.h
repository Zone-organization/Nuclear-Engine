#pragma once

namespace NuclearEngine
{
	struct DepthStencilStateDesc;
}

namespace NuclearRenderer {
	
	class NRBDepthStencilState
	{
	public:
		virtual void Create(NuclearEngine::DepthStencilStateDesc type) = 0;

		virtual void Delete() = 0;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;
	};
}