#pragma once
#include <Engine\Graphics\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class PBR : public RenderingPipeline
		{
		public:
			PBR();
			bool Bake(const RenderingPipelineDesc& desc) override;
			bool test = false;
		private:

		};

	}
}