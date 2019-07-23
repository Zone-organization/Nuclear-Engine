#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <unordered_map>

namespace NuclearEngine
{
	namespace Graphics
	{
		class NEAPI DynamicPipelineState
		{
		public:


			RefCntAutoPtr<IPipelineState> GetPipeline(Uint32 Key);
		private:
			std::unordered_map<Uint32, RefCntAutoPtr<IPipelineState>> mPipelineStates;
		};
	}
}