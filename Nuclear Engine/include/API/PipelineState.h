#pragma once
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBPipelineState;
}

namespace NuclearEngine {

	namespace API {
		class PipelineState
		{
		public:
			PipelineState(PipelineStateDesc type);
			~PipelineState();

			void Bind_DepthStencil();

			void Unbind_DepthStencil();
		private:
			NuclearRenderer::NRBPipelineState *state;

		};
	}
}