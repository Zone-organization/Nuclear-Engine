#pragma once
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace NuclearEngine
{
	namespace Graphics
	{

		class NEAPI BlinnPhong : public RenderingPipeline
		{
		public:
			BlinnPhong(bool NormalMaps = false);
			bool Bake(const RenderingPipelineDesc& desc) override;

		private:
			bool mNormalMaps;
		};

	}
}