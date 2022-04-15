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
			BlinnPhong(bool NormalMaps = false);
			bool Bake(const ShadingModelBakingDesc& desc) override;

		private:
			bool mNormalMaps;
		};

	}
}