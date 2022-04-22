#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI BlinnPhong : public ShadingModel
		{
		public:
			BlinnPhong();
			bool Bake(const ShadingModelBakingDesc& desc) override;

		private:
			void BakeGBufferPipeline(const ShadingModelBakingDesc& desc);
		};

	}
}