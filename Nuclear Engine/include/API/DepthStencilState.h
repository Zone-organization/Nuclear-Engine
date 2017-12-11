#pragma once
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBDepthStencilState;
}

namespace NuclearEngine {

	namespace API {
		class DepthStencilState
		{
		public:
			DepthStencilState();
			DepthStencilState(DepthStencilStateDesc type);
			~DepthStencilState();

			void Bind();

			void Unbind();
		private:
			NuclearRenderer::NRBDepthStencilState *state;

		};
	}
}