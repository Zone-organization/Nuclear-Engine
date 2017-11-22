#pragma once

namespace NuclearEngine
{
	struct PipelineStateDesc;
}

namespace NuclearRenderer {
	
	class NRBPipelineState
	{
		virtual void Create(NuclearEngine::PipelineStateDesc type) = 0;

		virtual void Delete() = 0;

		virtual void Bind_DepthStencil() = 0;

		virtual void Unbind_DepthStencil() = 0;
	};
}