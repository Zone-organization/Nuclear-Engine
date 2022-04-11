#pragma once
#include <Engine\ECS\System.h>
#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>
#include <unordered_map>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>

namespace Nuclear
{
	namespace Animation
	{
		class Animator;

	}

	namespace Components {
		class MeshComponent;
		class AnimatorComponent;
	}
	namespace Managers { class CameraManager; }
	namespace Systems
	{

		struct RenderSystemBakeStatus
		{
			bool BakeLighting = true;
			bool BakePipelines = true;
		};

		class NEAPI RenderSystem : public ECS::System<RenderSystem>
		{
		public:
			RenderSystem(Managers::CameraManager* CameraManager);
			~RenderSystem();

			void AddRenderingPipeline(Graphics::RenderingPipeline* Pipeline);
			void SetActiveRenderingPipeline(Uint32 PipelineID);

			void CreateMaterialForAllPipelines(Assets::Material* material);

			void CreateMaterial(Assets::Material* material, Uint32 PipelineID);

			bool NeedsBaking();

			void Bake(bool AllPipelines = true);

			IPipelineState* GetPipeline();

			// Render A MeshComponent Component instantly
			void InstantRender(const Components::MeshComponent& object);

			// Render A Mesh instantly
			void InstantRender(Assets::Mesh* mesh, Assets::Material* material);

			//Update Functions
			void Update(ECS::TimeDelta dt) override;

			//Debug Only
			bool VisualizePointLightsPositions = false;

		private:
			void RenderMeshes();

			LightingSubSystem mLightingSystem;
			Managers::CameraManager* mCameraManager;
			bool PipelineDirty = true;

			RenderSystemBakeStatus mStatus;
			RefCntAutoPtr<IBuffer> animCB;

			Assets::Mesh CameraScreenQuad;
			Assets::Material LightSphereMaterial;
			Graphics::RenderingPipeline* mActiveRenderingPipeline;
			std::unordered_map<Uint32, Graphics::RenderingPipeline*> mRenderingPipelines;
		};

	}
}