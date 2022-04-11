#pragma once
#include <Engine\Graphics\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class WireFrame : public RenderingPipeline
		{
		public:
			WireFrame();

			bool Bake(const RenderingPipelineDesc& desc) override;

		private:

		};

	}
}