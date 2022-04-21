#pragma once
#include <Engine/Rendering/RenderingPipeline.h>

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

			struct GBuffer
			{
				RefCntAutoPtr<ITexture> mPositonBuffer;
				RefCntAutoPtr<ITexture> mNormalBuffer;
				RefCntAutoPtr<ITexture> mAlbedoBuffer;
				RefCntAutoPtr<ITexture> pDepthBuffer;
			};
			GBuffer mGBuffer;

			void Initialize(const DefferedRenderingPipelineInitInfo& info);

			void Bake(const RenderingPipelineBakingDesc& desc) override;

			void ResizeRenderTarget(Uint32 Width, Uint32 Height) override;

			void SetPostProcessingEffect(const Uint32& effectId, bool value) override;

			void StartRendering(Systems::RenderSystem* renderer) override;

			void SetPipelineState() override;

		protected:
			//Graphics::RenderTarget BloomRT;
			//Rendering::BlurEffect mBloomBlur;

			//bool mBloomEnabled = false;

			
		private:
			void BakeRenderTargets();
			void BakePipeline();
			void RenderMeshes(Systems::RenderSystem* renderer);
		};
	}
}