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

			void StartStaticShaderModelRendering(ShadingModel* sm) override;

			void RenderStatic(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) override;

			void StartSkinnedShaderModelRendering(ShadingModel* sm) override;

			void RenderSkinned(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) override;

		private:
			FrameRenderData* pCurrentFrame;
			ShadingModel* pActiveShadingModel;

			void StartStaticRendering(ShadingModel* sm);
			void StartSkinnedRendering(ShadingModel* sm);
		};
	}
}