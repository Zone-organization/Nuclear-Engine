#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class PBR : public ShadingModel
		{
		public:
			PBR();
			bool Bake(const ShadingModelBakingDesc& desc) override;
			bool test = false;
		private:

		};

	}
}