#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\Light.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
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

			void BakeLightConstantBuffer();
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
			void Update_Light();
			void Update_Meshes(ECS::EntityManager & es);

			//Debug Only
			bool VisualizePointLightsPositions = false;

		private:
			RefCntAutoPtr<IBuffer> mPSLightCB;

			Components::CameraComponent* ActiveCamera;

			bool PipelineDirty = true;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;

			RenderSystemDesc mDesc;
			RenderSystemStatus mStatus;

			Assets::Material LightSphereMaterial;

			Graphics::RenderingPipeline* mRenderingPipeline;
		};

	}
}