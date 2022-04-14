#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class WireFrame : public ShadingModel
		{
		public:
			WireFrame();

			bool Bake(const ShadingModelBakingDesc& desc) override;

		private:

		};

	}
}