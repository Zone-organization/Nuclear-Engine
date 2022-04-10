#pragma once
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace NuclearEngine
{
	namespace Graphics
	{

		class NEAPI WireFrame : public RenderingPipeline
		{
		public:
			WireFrame();

			bool Bake(const RenderingPipelineDesc& desc) override;

		private:

		};

	}
}