#pragma once
#include <Rendering/RenderingPath.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI DefferedRenderingPath : public RenderingPath
		{
		public:
			void StartRendering(FrameRenderData* framedata, Graphics::ShaderPipelineVariant* pipeline) override;

			void Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) override;
		};
	}
}