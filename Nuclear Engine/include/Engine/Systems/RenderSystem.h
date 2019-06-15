#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\Light.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <vector>

namespace NuclearEngine
{
	namespace Systems
	{
		enum class NEAPI RenderSystemStatus
		{
			Ready,
			RequireBaking
		};

		struct NEAPI RenderSystemDesc
		{			
			bool NormalMaps = false;
			std::string VShaderPath = "NE_Default";
			std::string PShaderPath = "Assets/NuclearEngine/Shaders/BlinnPhong.ps.hlsl";

			//PostProcess Graphical Enchancements
			bool GammaCorrection = false;
			bool HDR = false;
		};

		class NEAPI RenderSystem : public ECS::System<RenderSystem> {
		public:
			RenderSystem(const RenderSystemDesc& desc = RenderSystemDesc(), Components::CameraComponent* camera = nullptr);
			~RenderSystem();

			void BakeLightConstantBuffer();
			void BakePipeline();
			void InitializePostProcessing(unsigned int WindowWidth = 1024, unsigned int WindowHeight = 768);
			void SetCamera(Components::CameraComponent* camera);
			Components::CameraComponent* GetCamera();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void CreateMaterial(Assets::Material* material);

			void Bake();

			IPipelineState* GetPipeline();

			//Main Rendering Function
			void Render();
			// Render A MeshComponent Component instantly
			void InstantRender(Components::MeshComponent* object);
			// Render A Mesh instantly
			void InstantRender(Assets::Mesh* mesh, Assets::Material* material);

			void RenderToPostProcessingRT();
			void RenderPostProcessingContents();

			//Update Functions
			void Update(ECS::EntityManager &es, ECS::EventManager &events, ECS::TimeDelta dt) override;
			void Update_Light();
			void Update_Meshes(ECS::EntityManager & es);

		private:
			void Calculate_Light_CB_Size();

			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IBuffer> mPSLightCB;

			Components::CameraComponent* ActiveCamera;

			bool PipelineDirty = true;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;

			RenderSystemDesc Desc;
			RenderSystemStatus status;

			//PostProcess stuff
			bool PostProcessingEnabled = false;
			ISampler* ScreenSampler;
			ITexture* PostProcessTexture;
			//IRenderTarget* PostProcessRT;
			IShader* PostProcess_VShader;
			IShader* PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};

	}
}