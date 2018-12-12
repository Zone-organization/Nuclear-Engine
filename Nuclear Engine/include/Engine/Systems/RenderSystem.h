#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\Light.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine\Graphics\Shader.h>
#include <LLGL\Sampler.h>
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

			void BakePipelines();
			void InitializePostProcessing(unsigned int WindowWidth = 1024, unsigned int WindowHeight = 768);
			void SetCamera(Components::CameraComponent* camera);
			Components::CameraComponent* GetCamera();

			Graphics::Shader GetVertexShader();
			Graphics::Shader GetPixelShader();

			void BindShaders();
			void BindConstantBuffers();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void Bake();

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

			LLGL::GraphicsPipeline* Pipeline = nullptr;

		private:
			void Calculate_Light_CB_Size();
			void BakePixelShader();
			void BakeVertexShader();

			Graphics::Shader VShader;
			Graphics::Shader PShader;
			LLGL::ShaderProgram* ShaderProgram;
			LLGL::Buffer* NE_Light_CB = nullptr;
			LLGL::PipelineLayout*   PipelineLayout = nullptr;

			Components::CameraComponent* ActiveCamera;

			bool VSDirty = true;
			bool PSDirty = true;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;

			RenderSystemDesc Desc;
			RenderSystemStatus status;

			//PostProcess stuff
			bool PostProcessingEnabled = false;
			LLGL::Sampler* ScreenSampler;
			LLGL::Texture* PostProcessTexture;
			LLGL::RenderTarget* PostProcessRT;
			Graphics::Shader PostProcess_VShader;
			Graphics::Shader PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};

	}
}