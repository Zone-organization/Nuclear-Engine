#pragma once
#include <Engine\ECS\System.h>
#include <Engine\Rendering\ShadingModel.h>
#include <Engine\Rendering\RenderingPipeline.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <Engine\Rendering\Background.h>
#include <vector>
#include <unordered_map>
#include <Engine/Rendering/RenderPass/RenderPass.h>
#include <Core/Logger.h>
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

			void AddRenderingPipeline(Rendering::RenderingPipeline* Pipeline);
			void SetActiveRenderingPipeline(Uint32 PipelineID);

			bool NeedsBaking();

			void Bake(Uint32 RTWidth, Uint32 RTHeight, bool AllPipelines = true);

			void ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight);

			Rendering::RenderingPipeline* GetActivePipeline();

			//////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////WIP - RENDER PASS/////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////
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
			//////////////////////////////////////////////////////////////////////////////////////////////

			//Update Functions
			void Update(ECS::TimeDelta dt) override;

			IBuffer* GetAnimationCB();

			CameraSystem* GetCameraSystem();

			Rendering::Background& GetBackground();


			/////////////////////////////////////////////////////////////////////////////////////////
			IBuffer* GetLightCB();

			Rendering::FrameRenderData mRenderData;
			size_t GetDirLightsNum();
			size_t GetPointLightsNum();
			size_t GetSpotLightsNum();
		private:
			Rendering::Background mBackground;
			std::vector<Rendering::RenderPass*> mRenderPasses;

			std::shared_ptr<CameraSystem> mCameraSystemPtr;
			std::shared_ptr<LightingSystem> mLightingSystemPtr;

			RenderSystemBakeStatus mStatus;
			RefCntAutoPtr<IBuffer> mAnimationCB;

			Rendering::RenderingPipeline* mActiveRenderingPipeline;
			std::unordered_map<Uint32, Rendering::RenderingPipeline*> mRenderingPipelines;


			/////////////////////////////////////////////////////////////////////////////////////////
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