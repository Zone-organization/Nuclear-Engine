#pragma once
#include <Engine\Graphics\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class DiffuseOnly : public RenderingPipeline
		{
		public:
			DiffuseOnly();

			bool Bake(const RenderingPipelineDesc& desc) override;

		private:

		};

	}
}