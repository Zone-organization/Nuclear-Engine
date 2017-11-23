#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBPipelineState.h>

namespace NuclearRenderer {

	class NROGL3API GLPipelineState : public NRBPipelineState
	{
	public:
		GLPipelineState();

		void Create(NuclearEngine::PipelineStateDesc type) override;

		void Delete() override;

		void Bind_DepthStencil() override;

		void Unbind_DepthStencil() override;
	protected:
		NuclearEngine::PipelineStateDesc *desc;
		unsigned int count;

		GLboolean depthmask = false;
		GLenum depthfunc;

		GLubyte facesmask;

	};
}