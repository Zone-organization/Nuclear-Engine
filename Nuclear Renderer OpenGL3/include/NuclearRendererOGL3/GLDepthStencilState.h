#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBDepthStencilState.h>

namespace NuclearRenderer {

	class NROGL3API GLDepthStencilState : public NRBDepthStencilState
	{
	public:
		GLDepthStencilState();

		void Create(NuclearEngine::DepthStencilStateDesc type) override;

		void Delete() override;

		void Bind() override;

		void Unbind() override;
	protected:
		NuclearEngine::DepthStencilStateDesc *desc;
		unsigned int count;

		GLboolean depthmask = false;
		GLenum depthfunc;

		GLubyte facesmask;

	};
}