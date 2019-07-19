#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>
#include <unordered_map>

namespace NuclearEngine
{
	namespace Assets
	{
		class Mesh;
		class Material;
	}

	namespace Components {
		class MeshComponent;
	}
	namespace Managers { class CameraManager; }
	namespace Systems
	{

		struct RenderSystemBakeStatus
		{
			bool BakeLighting = true;
			bool BakePipelines = true;
		};

		class NEAPI RenderSystem : public ECS::System<RenderSystem> {
		public:
			RenderSystem(Managers::CameraManager* CameraManager);
			~RenderSystem();

			void AddRenderingPipeline(Graphics::RenderingPipeline* Pipeline);
			void SetActiveRenderingPipeline(Uint32 PipelineID);

			void CreateMaterialForAllPipelines(Assets::Material* material);

			void CreateMaterial(Assets::Material* material, Uint32 PipelineID);

			bool NeedsBaking();

			void Bake(ECS::EntityManager& es, bool AllPipelines = true);

			IPipelineState* GetPipeline();

			// Render A MeshComponent Component instantly
			void InstantRender(Components::MeshComponent* object);
			// Render A Mesh instantly
			void InstantRender(Assets::Mesh* mesh, Assets::Material* material);

			//Update Functions
			void Update(ECS::EntityManager &es, ECS::EventManager &events, ECS::TimeDelta dt) override;
			void UpdateMeshes(ECS::EntityManager & es);

			//Debug Only
			bool VisualizePointLightsPositions = false;

		private:
			LightingSubSystem mLightingSystem;
			Managers::CameraManager* mCameraManager;
			bool PipelineDirty = true;

			RenderSystemBakeStatus mStatus;

			Assets::Mesh CameraScreenQuad;
			Assets::Material LightSphereMaterial;
			Graphics::RenderingPipeline* mActiveRenderingPipeline;
			std::unordered_map<Uint32, Graphics::RenderingPipeline*> mRenderingPipelines;
		};

	}
}