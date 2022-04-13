#pragma once
#include <Engine\ECS\System.h>
#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Systems\CameraSubSystem.h>
#include <Engine\Graphics\RenderingPipeline.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <vector>
#include <unordered_map>

namespace Nuclear
{
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
			RenderSystem(Graphics::Camera *startingcamera);
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

			CameraSubSystem& GetCameraSubSystem();

		private:
			void RenderMeshes();

			LightingSubSystem mLightingSystem;
			CameraSubSystem mCameraSystem;

			RenderSystemBakeStatus mStatus;
			RefCntAutoPtr<IBuffer> animCB;

			Assets::Mesh CameraScreenQuad;
			Assets::Material LightSphereMaterial;
			Graphics::RenderingPipeline* mActiveRenderingPipeline;
			std::unordered_map<Uint32, Graphics::RenderingPipeline*> mRenderingPipelines;
		};

	}
}