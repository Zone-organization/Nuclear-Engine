#pragma once
#include <Engine\Rendering\ShadingModel.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class DiffuseOnly : public ShadingModel
		{
		public:
			DiffuseOnly();

			bool Bake(const ShadingModelBakingDesc& desc) override;

		private:

		};

	}
}