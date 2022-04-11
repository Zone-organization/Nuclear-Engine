#pragma once
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class NEAPI PBR : public RenderingPipeline
		{
		public:
			PBR();
			bool Bake(const RenderingPipelineDesc& desc) override;

		private:

		};

	}
}