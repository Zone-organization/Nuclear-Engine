//#pragma once
//#include <Core\NE_Common.h>
//#include <Engine/Graphics/Camera.h>
//#include <Engine/Rendering/ShaderEffect.h>
//#include <Engine/Rendering/ShadingModel.h>
//#include <Engine/Graphics/BakeStatus.h>
//#include <Engine/Rendering/CSMShadowMap.h>
//#include <Engine\Systems\CameraSubSystem.h>
//#include <Engine\ECS\Scene.h>
//#include <unordered_map>
//
//namespace Nuclear
//{
//	namespace Rendering
//	{
//		class NEAPI DefferedRenderingPipeline
//		{
//		public:
//			DefferedRenderingPipeline(const std::string& name);
//
//			void Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera, bool initshadow = false);
//
//			std::unordered_map<Uint32, ShaderEffect> mPairedEffects;
//
//			void SetEffect(const Uint32& effectId, bool value);
//
//			Rendering::ShadingModel* GetShadingModel();
//			Graphics::Camera* GetCamera();
//
//			void Bake(ShadingModelBakingDesc& ModelDesc, Uint32 RTWidth, Uint32 RTHeight);
//
//			Uint32 GetID() const;
//			std::string GetName() const;
//
//			Graphics::BakeStatus GetStatus();
//
//			//Camera stuff
//			Graphics::RenderTarget* GetSceneRT();
//			IPipelineState* GetActivePipeline();
//			IShaderResourceBinding* GetActiveSRB();
//
//			//void Bake(Uint32 RTWidth, Uint32 RTHeight);
//			void ResizeRenderTarget(Uint32 Width, Uint32 Height);
//
//			void UpdatePSO(bool ForceDirty = false);
//
//			void SetPostProcessingEffect(const Uint32& effectId, bool value);
//
//			void StartRendering(ECS::Scene* mScene, Systems::CameraSubSystem* camerasystem, IBuffer* AnimCB);
//			void ApplyPostProcessingEffects();
//
//			void SetPipelineState();
//
//			// Render A Mesh instantly
//			void InstantRender(Assets::Mesh* mesh, Assets::Material* material, bool shadowpass);
//
//		protected:
//			Rendering::ShadingModel* mShadingModel;
//			Graphics::Camera* mCamera;
//			Uint32 mID;
//			std::string mName;
//
//			std::unordered_map<Uint32, Rendering::ShaderEffect> mPostProcessingEffects;
//
//			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;
//
//			//Animation
//
//			//Camera stuff
//			Graphics::RenderTarget SceneRT;
//			Graphics::RenderTarget BloomRT;
//
//			Graphics::CompoundPipeline mPipeline;
//
//			RefCntAutoPtr<IPipelineState> mActivePSO;
//			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;
//
//			Rendering::BlurEffect mBloomBlur;
//
//			bool mPipelineDirty = true;
//			bool mBloomEnabled = false;
//			Uint32 RequiredHash = 0;
//
//			std::string VS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.vs.hlsl";
//			std::string PS_Path = "Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl";
//
//			Uint32 RTWidth = 0;
//			Uint32 RTHeight = 0;
//
//			void BakeRenderTarget();
//			void BakePipeline();
//
//			//Shadow
//			ShadowMapManager m_ShadowMapMgr;
//			RefCntAutoPtr<ISampler> m_pComparisonSampler;
//
//		};
//	}
//}