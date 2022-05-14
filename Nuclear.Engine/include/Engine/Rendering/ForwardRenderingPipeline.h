#pragma once
#include <Engine/Rendering/RenderingPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI ForwardRenderingPipeline : public RenderingPipeline
		{
		public:
			void Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera);

			void StartRendering(Systems::RenderSystem* renderer) override;
		};
	}
}