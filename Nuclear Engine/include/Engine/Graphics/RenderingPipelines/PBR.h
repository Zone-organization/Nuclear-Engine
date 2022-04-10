#pragma once
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace NuclearEngine
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