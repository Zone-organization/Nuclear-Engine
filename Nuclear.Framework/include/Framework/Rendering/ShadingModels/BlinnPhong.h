#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class BlinnPhong : public ShadingModel
		{
		public:
			BlinnPhong(bool NormalMaps = false);
			bool Bake(const ShadingModelBakingDesc& desc) override;

		private:
			bool mNormalMaps;
		};

	}
}