#pragma once
#include <Engine/Rendering/RenderingPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI ForwardRenderingPipeline : public RenderingPipeline
		{
		public:
			void RenderQueue(FrameRenderData* frame, DrawQueue* queue) override;
		};
	}
}