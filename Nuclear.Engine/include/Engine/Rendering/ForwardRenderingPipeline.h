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

			void Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera, bool initshadow = false);

			std::unordered_map<Uint32, ShaderEffect> mPairedEffects;

			void SetEffect(const Uint32& effectId, bool value);


			void Bake(const RenderingPipelineBakingDesc& desc) override;

			//Camera stuff
			Graphics::RenderTarget* GetSceneRT();
			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

			//void Bake(Uint32 RTWidth, Uint32 RTHeight);
			void ResizeRenderTarget(Uint32 Width, Uint32 Height);

			void UpdatePSO(bool ForceDirty = false);

			void SetPostProcessingEffect(const Uint32& effectId, bool value);

			void StartRendering(ECS::Scene* mScene, Systems::CameraSubSystem* camerasystem, IBuffer* AnimCB);
			void ApplyPostProcessingEffects();

			void SetPipelineState();

			// Render A Mesh instantly
			void InstantRender(Assets::Mesh* mesh, Assets::Material* material, bool shadowpass);

		protected:
			std::unordered_map<Uint32, Rendering::ShaderEffect> mPostProcessingEffects;

			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;


			//Camera stuff
			Graphics::RenderTarget SceneRT;
			Graphics::RenderTarget BloomRT;

			Graphics::CompoundPipeline mPipeline;

			RefCntAutoPtr<IPipelineState> mActivePSO;
			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;

			Rendering::BlurEffect mBloomBlur;

			bool mBloomEnabled = false;

			std::string VS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.vs.hlsl";
			std::string PS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl";


			//Shadow
			ShadowMapManager m_ShadowMapMgr;
			RefCntAutoPtr<ISampler> m_pComparisonSampler;

		private:
			void BakeRenderTarget();
			void BakePipeline();
		};
	}
}