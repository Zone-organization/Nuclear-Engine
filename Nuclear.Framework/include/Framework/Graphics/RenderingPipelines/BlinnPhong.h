#pragma once
#include <Engine\Graphics\RenderingPipeline.h>
#include <vector>

namespace Nuclear
{
	namespace Graphics
	{

		class BlinnPhong : public RenderingPipeline
		{
		public:
			BlinnPhong(bool NormalMaps = false);
			bool Bake(const RenderingPipelineDesc& desc) override;

		private:
			bool mNormalMaps;
		};

	}
}