#pragma once
#include <Engine/Rendering/RenderingPipeline.h>
#include <Engine/Rendering/GBuffer.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct DefferedRenderingPipelineInitInfo
		{
			Rendering::ShadingModel* shadingModel;
			Graphics::Camera* camera;

			TEXTURE_FORMAT mDepthBufferFormat = TEX_FORMAT_D16_UNORM;
		};
		class NEAPI DefferedRenderingPipeline : public RenderingPipeline
		{
		public:
			DefferedRenderingPipeline(const std::string& name);

			void Initialize(const DefferedRenderingPipelineInitInfo& info);

			//void Bake(const RenderingPipelineBakingDesc& desc) override;

			void ResizeRenderTargets(Uint32 Width, Uint32 Height) override;

			void StartRendering(Systems::RenderSystem* renderer) override;

		private:
			GBuffer mGBuffer;

			void BakeRenderTargets() override;
			void RenderMeshes(Systems::RenderSystem* renderer);
		};
	}
}