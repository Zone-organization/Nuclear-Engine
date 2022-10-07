#pragma once
#include <Engine\ECS\System.h>
#include <vector>
#include <unordered_map>
#include <Engine/Rendering/RenderPass.h>
#include <Engine/Rendering/FrameRenderData.h>

namespace Nuclear
{
	namespace Systems
	{
		class CameraSystem;

		struct RenderSystemBakeStatus
		{
			bool BakeLighting = true;
			bool BakePipelines = true;
		};

		class NEAPI RenderSystem : public ECS::System<RenderSystem>
		{
		public:
			RenderSystem();
			~RenderSystem();

			bool NeedsBaking();

			void Bake(Uint32 RTWidth, Uint32 RTHeight, bool AllPipelines = true);

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight);

			void AddRenderPass(Rendering::RenderPass* pass);

			template <typename T>
			T* GetRenderPass()
			{
				static_assert(std::is_base_of<Rendering::RenderPass, T>::value, "GetRenderPass<T> class must derive from IRenderPass!");

				for (Rendering::RenderPass* pass : mRenderPasses)
				{
					T* result = dynamic_cast<T*>(pass);
					if (result)
					{
						return result;
					}
				}
				return nullptr;
			}

			//Update Functions
			void Update(ECS::TimeDelta dt) override;

			IBuffer* GetAnimationCB();

			CameraSystem* GetCameraSystem();

			void RegisterShadingModel(Rendering::ShadingModel* shadingmodel);

			/////////////////////////////////////////////////////////////////////////////////////////
			IBuffer* GetLightCB();

			Rendering::FrameRenderData mRenderData;
			size_t GetDirLightsNum();
			size_t GetPointLightsNum();
			size_t GetSpotLightsNum();
		private:
			std::vector<Rendering::RenderPass*> mRenderPasses;
			std::vector<Rendering::ShadingModel*> mRegisteredShadingModels;

			std::shared_ptr<CameraSystem> mCameraSystemPtr;

			RenderSystemBakeStatus mStatus;
			RefCntAutoPtr<IBuffer> mAnimationCB;

			RefCntAutoPtr<IPipelineState> pSceneToScreenPSO;
			RefCntAutoPtr<IShaderResourceBinding> pSceneToScreenSRB;

			void BakeScenePipeline(Uint32 RTWidth, Uint32 RTHeight);

			size_t Baked_DirLights_Size = 0;
			size_t Baked_PointLights_Size = 0;
			size_t Baked_SpotLights_Size = 0;
			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			bool HasbeenBakedBefore = false;

			RefCntAutoPtr<IBuffer> mPSLightCB;

			void BakeLights();
			void BakeLightsBuffer();
			void UpdateLights();
			bool LightRequiresBaking();
			void UpdateLightsBuffer(const Math::Vector4& CameraPos);
		};

	}
}