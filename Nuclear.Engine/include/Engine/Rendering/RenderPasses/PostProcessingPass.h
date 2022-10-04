#pragma once

#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Rendering\ShadingModel.h>
#include <Engine\Rendering\RenderPass.h>
#include <vector>
#include <Engine\ECS\System.h>
#include <Engine/Rendering/RenderingPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{

		struct PostProcessingPassDesc {
			std::string PostFX_VS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.vs.hlsl";
			std::string PostFX_PS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl";
		};

		struct PostProcessingBakingDesc
		{
			Uint32 mRTWidth;
			Uint32 mRTHeight;
		};


		//TODO: Should separate bloom color values after geometrypass finishes
		//TODO: Should Get rid of compound pipeline and switch to baked one since the effects toggle rarely change
		class NEAPI PostProcessingPass : public RenderPass
		{
		public:
			PostProcessingPass();


			//Camera stuff
			Graphics::RenderTarget* GetSceneRT();
			Graphics::RenderTarget* GetSceneDepthRT();

			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

			virtual void Bake(const PostProcessingBakingDesc& desc);

			virtual void SetFinalPipelineState();


			//Camera stuff
			//Graphics::RenderTarget SceneRT;
			//Graphics::RenderTarget SceneDepthRT;

			RefCntAutoPtr<IPipelineState> mActivePSO;
			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;


			virtual void UpdatePSO(bool ForceDirty = false);

			void Initialize(RenderingPipeline* pipeline);

			void Update(FrameRenderData* framedata) override;

			virtual void ResizeRenderTargets(Uint32 Width, Uint32 Height);

			virtual void SetPostProcessingEffect(const Uint32& effectId, bool value);

			virtual void BakePostFXPipeline();
			virtual void BakeRenderTargets();
		protected:
			PostProcessingPassDesc mDesc;

			//PostProcessing Effects
			std::unordered_map<Uint32, Rendering::ShaderEffect> mPostProcessingEffects;
			Graphics::CompoundPipeline mPostFXPipeline;


			Graphics::RenderTarget BloomRT;
			Rendering::BlurEffect mBloomBlur;
			RefCntAutoPtr<IPipelineState> pBloomExtractPSO;
			RefCntAutoPtr<IShaderResourceBinding> pBloomExtractSRB;

			bool mBloomEnabled = false;

			bool mPipelineDirty = true;
			Uint32 mRequiredHash = 0;
			Uint32 mRTWidth = 0;
			Uint32 mRTHeight = 0;
		};

	}
}