#pragma once
#include <Engine/Rendering/RenderingPipeline.h>
#include <Engine/Rendering/GBuffer.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI DefferedRenderingPipeline : public RenderingPipeline
		{
		public:
			void RenderQueue(FrameRenderData* frame, DrawQueue* queue) override;

		private:
			void RenderMeshes(FrameRenderData* frame, DrawQueue* queue);
		};
	}
}