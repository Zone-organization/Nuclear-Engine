#pragma once
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearEngine {

	class PipelineState
	{
	public:
		void Create(PipelineStateDesc type);

		void Delete();

		void Bind_DepthStencil();

		void Unbind_DepthStencil();
	};
}