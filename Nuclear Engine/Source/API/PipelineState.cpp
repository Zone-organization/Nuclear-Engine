#include <NuclearRendererBase\NRBPipelineState.h>
#include <Core\Context.h>
#include <API\PipelineState.h>

namespace NuclearEngine {
	namespace API {

		PipelineState::PipelineState(PipelineStateDesc type)
		{
			state = Core::Context::ConstructPipelineState(state);

			state->Create(type);
		}
		PipelineState::~PipelineState()
		{
			state->Delete();

			delete state;
		}
		void PipelineState::Bind_DepthStencil()
		{
			state->Bind_DepthStencil();
		}
		void PipelineState::Unbind_DepthStencil()
		{
			state->Unbind_DepthStencil();
		}
	}
}