#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Rendering/CSMShadowMap.h>
#include <Engine\ECS\Scene.h>
#include <unordered_map>

namespace Nuclear
{
	namespace Systems {
		class RenderSystem;
	}

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

			Uint32 GetID() const;
			std::string GetName() const;
			Rendering::ShadingModel* GetShadingModel() const;
			Graphics::Camera* GetCamera() const;
			Graphics::BakeStatus GetStatus() const;

			//Camera stuff
			Graphics::RenderTarget* GetSceneRT();
			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

			virtual void Bake(const RenderingPipelineBakingDesc& desc) = 0;

			virtual void ResizeRenderTarget(Uint32 Width, Uint32 Height) = 0;

			virtual void SetPostProcessingEffect(const Uint32& effectId, bool value) = 0;

			virtual void StartRendering(Systems::RenderSystem* renderer) = 0;

			virtual void SetPipelineState() = 0;

			virtual void UpdatePSO(bool ForceDirty = false);

			// Render A Mesh instantly
			virtual void InstantRender(Assets::Mesh* mesh, Assets::Material* material);

		protected:
			std::unordered_map<Uint32, Rendering::ShaderEffect> mPostProcessingEffects;

			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;

			void SetShadingModelAndCamera(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera);

			//Camera stuff
			Graphics::RenderTarget SceneRT;

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