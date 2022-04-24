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

namespace Nuclear
{
	namespace Systems
	{
		class CameraSystem;
		class LightingSystem;

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

			void CreateMaterialForAllPipelines(Assets::Material* material);

			void CreateMaterial(Assets::Material* material, Uint32 PipelineID);

			bool NeedsBaking();

			void Bake(Uint32 RTWidth, Uint32 RTHeight, bool AllPipelines = true);

			void ResizeRenderTargets(Uint32 RTWidth, Uint32 RTHeight);

			Rendering::RenderingPipeline* GetActivePipeline();

			//Update Functions
			void Update(ECS::TimeDelta dt) override;

			//Debug Only
			bool VisualizePointLightsPositions = false;

			IBuffer* GetAnimationCB();
			Assets::Material LightSphereMaterial;                     //TODO: Move

			CameraSystem* GetCameraSystem();
			LightingSystem* GetLightingSystem();

			Rendering::Background& GetBackground();

		private:
			Rendering::Background mBackground;

			std::shared_ptr<CameraSystem> mCameraSystemPtr;
			std::shared_ptr<LightingSystem> mLightingSystemPtr;

			RenderSystemBakeStatus mStatus;
			RefCntAutoPtr<IBuffer> mAnimationCB;

			Rendering::RenderingPipeline* mActiveRenderingPipeline;
			std::unordered_map<Uint32, Rendering::RenderingPipeline*> mRenderingPipelines;
		};

	}
}