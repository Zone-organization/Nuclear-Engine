#pragma once
#include <Engine\Rendering\RenderPass.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI DefferedPass : public RenderPass
		{
		public:
			DefferedPass();

			void Update(FrameRenderData* framedata) override;
		};

	}
}