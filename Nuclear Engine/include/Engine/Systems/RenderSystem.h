#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace NuclearEngine
{
//Forward Declarations
	namespace Assets {
		class Mesh;
		class Material;
	}

	namespace Components {
		class CameraComponent;
		class MeshComponent;
	}

	namespace Systems
	{
		enum class NEAPI RenderSystemStatus
		{
			Ready,
			RequireBaking
		};

		struct NEAPI RenderSystemDesc
		{
			//TODO
			bool AllowMultipleRenderingPipelines = true;
			bool CreateMaterialInstanceForEachPipeline = true;
		};

		class NEAPI RenderSystem : public ECS::System<RenderSystem> {
		public:
			RenderSystem(const RenderSystemDesc& desc = RenderSystemDesc(), Components::CameraComponent* camera = nullptr);
			~RenderSystem();

			void SetRenderingPipeline(Graphics::RenderingPipeline* Pipeline);

			void SetCamera(Components::CameraComponent* camera);
			Components::CameraComponent* GetCamera();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void CreateMaterial(Assets::Material* material);

			void Bake();

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

			Components::CameraComponent* ActiveCamera;

			bool PipelineDirty = true;

			RenderSystemDesc mDesc;
			RenderSystemStatus mStatus;

			Assets::Material LightSphereMaterial;

			Graphics::RenderingPipeline* mRenderingPipeline;
		};

	}
}