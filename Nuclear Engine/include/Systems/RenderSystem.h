#pragma once
#include <NE_Common.h>
#include <ECS/Entity.h>
#include "ECS/Event.h"
#include "ECS/System.h"
#include <Components\MeshComponent.h>
#include <Components\GenericCamera.h>
#include <Components\Light.h>
#include <Assets\Mesh.h>
#include <Graphics/API/RenderTarget.h>
#include <Graphics/API/Sampler.h>
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
			RenderSystem(const RenderSystemDesc& desc = RenderSystemDesc());
			~RenderSystem();

			void InitializePostProcessing(unsigned int WindowWidth = 1024, unsigned int WindowHeight = 768);
			void SetCamera(Components::GenericCamera* camera);

			Graphics::API::VertexShader GetVertexShader();
			Graphics::API::PixelShader GetPixelShader();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void Bake();

			//Main Rendering Function
			void Render();
			// Render A MeshComponent Component instantly
			void InstantRender(Components::MeshComponent* object);
			// Render A SubMesh instantly
			void InstantRender(Assets::Mesh::SubMesh* submesh);
			// Render A Mesh instantly
			void InstantRender(Assets::Mesh* mesh);

			void RenderToPostProcessingRT();
			void RenderPostProcessingContents();

			//Update Functions
			void Update(ECS::EntityManager &es, ECS::EventManager &events, ECS::TimeDelta dt) override;
			void Update_Light();

		private:
			void Calculate_Light_CB_Size();
			void BakePixelShader();
			void BakeVertexShader();

			Graphics::API::VertexShader VShader;
			Graphics::API::PixelShader PShader;
			Graphics::API::ConstantBuffer NE_Light_CB;

			Components::GenericCamera* ActiveCamera;

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
			Graphics::API::Sampler ScreenSampler;
			Graphics::API::Texture PostProcessTexture;
			Graphics::API::RenderTarget PostProcessRT;
			Graphics::API::VertexShader PostProcess_VShader;
			Graphics::API::PixelShader PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};

	}
}