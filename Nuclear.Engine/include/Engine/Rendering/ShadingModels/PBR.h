#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class NEAPI PBR : public ShadingModel
		{
		public:
			PBR();
			bool Bake(const ShadingModelBakingDesc& desc) override;
			bool test = false;
		private:

		};

	}
}