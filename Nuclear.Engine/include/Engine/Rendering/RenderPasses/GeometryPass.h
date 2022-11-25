#pragma once
#include <Engine\Rendering\RenderPass.h>
#include <Engine\Rendering\RenderingPath.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct FrameRenderData;
		class ShaderPipeline;
		class NEAPI GeometryPass : public RenderPass
		{
		public:
			GeometryPass();

			void Update(FrameRenderData* framedata) override;

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight) override;

			RenderingPath* pRenderingPath;
		protected:

			ShaderPipeline* pActivePipeline = nullptr;
			FrameRenderData* pCurrentFrame = nullptr;
		};

	}
}