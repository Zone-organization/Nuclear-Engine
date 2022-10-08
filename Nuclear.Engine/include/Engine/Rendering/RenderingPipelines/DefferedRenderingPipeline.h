#pragma once
#include <Engine/Rendering/RenderingPipeline.h>
#include <Engine/Rendering/GBuffer.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI DefferedRenderingPipeline : public RenderingPipeline
		{
		public:

			void BeginFrame(FrameRenderData* frame) override;

			void StartShaderModelRendering(ShadingModel* sm) override;

			void Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) override;

			void FinishShaderModelRendering() override;

		private:
			FrameRenderData* pCurrentFrame;
			ShadingModel* pActiveShadingModel;
		};
	}
}