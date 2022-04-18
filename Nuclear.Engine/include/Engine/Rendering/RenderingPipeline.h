#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Rendering/CSMShadowMap.h>
#include <Engine\Systems\CameraSubSystem.h>
#include <Engine\ECS\Scene.h>
#include <unordered_map>

namespace Nuclear
{
	namespace Rendering
	{
		struct RenderingPipelineBakingDesc
		{
			ShadingModelBakingDesc mShadingModelDesc;
			Uint32 mRTWidth;
			Uint32 mRTHeight;
		};

		//A class of compatible camera and shading model
		//Defines how to render stuff and apply effects to it
		class NEAPI RenderingPipeline
		{
		public:
			RenderingPipeline(const std::string& name);

			std::unordered_map<Uint32, ShaderEffect> mPairedEffects;

			void SetEffect(const Uint32& effectId, bool value);

			virtual void Bake(const RenderingPipelineBakingDesc& desc) = 0;

			Uint32 GetID() const;
			std::string GetName() const;
			Rendering::ShadingModel* GetShadingModel() const;
			Graphics::Camera* GetCamera() const;

			Graphics::BakeStatus GetStatus() const;

			//Camera stuff
			Graphics::RenderTarget* GetSceneRT();
			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

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

			void SetShadingModelAndCamera(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera);

			//Camera stuff
			Graphics::RenderTarget SceneRT;
			Graphics::RenderTarget BloomRT;

			Graphics::CompoundPipeline mPipeline;

			RefCntAutoPtr<IPipelineState> mActivePSO;
			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;


			bool mPipelineDirty = true;
			Uint32 mRequiredHash = 0;
			Uint32 mRTWidth = 0;
			Uint32 mRTHeight = 0;

		private:
			Rendering::ShadingModel* mShadingModel;
			Graphics::Camera* mCamera;
			Uint32 mID;
			std::string mName;
		};
	}
}