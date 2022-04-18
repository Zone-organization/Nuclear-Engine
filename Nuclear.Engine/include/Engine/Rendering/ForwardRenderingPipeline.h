#pragma once
#include <Engine/Rendering/RenderingPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI ForwardRenderingPipeline : public RenderingPipeline
		{
		public:
			ForwardRenderingPipeline(const std::string& name);

			void Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera);

			void Bake(const RenderingPipelineBakingDesc& desc) override;

			void ResizeRenderTarget(Uint32 Width, Uint32 Height) override;

			void SetPostProcessingEffect(const Uint32& effectId, bool value) override;

			void StartRendering(Systems::RenderSystem* renderer) override;

			void SetPipelineState() override;

		protected:
			Graphics::RenderTarget BloomRT;
			Rendering::BlurEffect mBloomBlur;

			bool mBloomEnabled = false;

			std::string VS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.vs.hlsl";
			std::string PS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl";
		private:
			void BakeRenderTarget();
			void BakePipeline();
			void ApplyPostProcessingEffects();
		};
	}
}