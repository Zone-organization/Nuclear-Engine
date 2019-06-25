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
		class NEAPI LightingSubSystem 
		{
		public:


			void BakeLightConstantBuffer();
			void SetCamera(Components::CameraComponent* camera);
			Components::CameraComponent* GetCamera();

			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;

			void BakeBuffer();

			//Update Functions
			void UpdateBuffer();

			//Debug Only
			bool VisualizePointLightsPositions = false;

		private:
			RefCntAutoPtr<IBuffer> mPSLightCB;

			Components::CameraComponent* ActiveCamera;

			bool PipelineDirty = true;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			Assets::Material LightSphereMaterial;

			Graphics::RenderingPipeline* mRenderingPipeline;
		};

	}
}