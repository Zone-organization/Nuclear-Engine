#pragma once
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Rendering\RenderPass.h>
#include <vector>
#include <ECS\System.h>
#include <Rendering/BlurEffect.h>
#include <Graphics/ShaderPipeline.h>
#include <Rendering\Background.h>

namespace Nuclear
{
	namespace Rendering
	{

		struct PostProcessingPassDesc {
			std::string PostFX_VS_Path = "@NuclearAssets@/Shaders/PostProcessing.vs.hlsl";
			std::string PostFX_PS_Path = "@NuclearAssets@/Shaders/PostProcessing.ps.hlsl";
		};

		struct PostProcessingBakingDesc
		{
			Uint32 mRTWidth;
			Uint32 mRTHeight;
			Graphics::RenderTargetDesc mFinalRTDesc;
		};


		//TODO: Should Get rid of compound pipeline and switch to baked one since the effects toggle rarely change
		class NEAPI PostProcessingPass : public RenderPass
		{
		public:
			PostProcessingPass();

			virtual void Bake(const PostProcessingBakingDesc& desc);

			void Update(FrameRenderData* framedata) override;

			void ResizeRTs(Uint32 Width, Uint32 Height) override;

			void SetPostProcessingEffect(Uint32 effectId, bool value);

			Graphics::ShaderPipelineSwitchController& GetPipelineController();

			virtual void BakePostFXPipeline();

			Rendering::Background& GetBackground();
		protected:
			PostProcessingPassDesc mDesc;

			//PostProcessing Effects
			Graphics::ShaderPipelineDesc PSOCreateInfo;
			Graphics::ShaderPipeline mPostFXPipeline;
			Graphics::ShaderPipelineSwitchController mPipelineCntrllr;
			Graphics::RenderTarget PostFXRT;

			Graphics::RenderTarget BloomRT;
			Rendering::BlurEffect mBloomBlur;
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pBloomExtractPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pBloomExtractSRB;

			bool mBloomEnabled = false;

			bool mPipelineDirty = true;

			Uint32 mRTWidth = 0;
			Uint32 mRTHeight = 0;

			Uint32 _HashedBloomID;

			Rendering::Background mBackground;
		};

	}
}