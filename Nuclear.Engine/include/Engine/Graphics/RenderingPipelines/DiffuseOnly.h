#pragma once
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class NEAPI DiffuseOnly : public RenderingPipeline
		{
		public:
			DiffuseOnly();

			bool Bake(const RenderingPipelineDesc& desc) override;

		private:

		};

	}
}