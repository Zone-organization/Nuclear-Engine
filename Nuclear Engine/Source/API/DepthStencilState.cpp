#include <NuclearRendererBase\NRBDepthStencilState.h>
#include <Core\Context.h>
#include <API\DepthStencilState.h>

namespace NuclearEngine {
	namespace API {

		DepthStencilState::DepthStencilState(DepthStencilStateDesc type)
		{
			state = Core::Context::ConstructDepthStencilState(state);

			state->Create(type);
		}
		DepthStencilState::~DepthStencilState()
		{
			state->Delete();

			delete state;
		}
		void DepthStencilState::Bind()
		{
			state->Bind();
		}
		void DepthStencilState::Unbind()
		{
			state->Unbind();
		}
	}
}