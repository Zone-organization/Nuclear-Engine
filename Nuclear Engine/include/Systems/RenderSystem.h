#pragma once
#include <NE_Common.h>
#include <Core/Entity.h>
#include "Core/Event.h"
#include "Core/System.h"
#include <Components\Model.h>
#include <Components\GenericCamera.h>
#include <Components\Light.h>
#include <XAsset\ModelAsset.h>
#include <API\RenderTarget.h>
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

		class RenderSystem : public Core::System<RenderSystem> {
		public:
			RenderSystem(const RenderSystemDesc& desc = RenderSystemDesc());
			~RenderSystem();

			void InitializePostProcessing(unsigned int WindowWidth = 1024, unsigned int WindowHeight = 768);
			void SetCamera(Components::GenericCamera* camera);

			API::VertexShader GetVertexShader();
			API::PixelShader GetPixelShader();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void Bake();

			//Main Rendering Function
			void Render();
			// Render A Model Component instantly
			void InstantRender(Components::Model* object);
			// Render A Mesh Component instantly
			void InstantRender(XAsset::Mesh* mesh);

			//Update Functions
			void Update(Core::EntityManager &es, Core::EventManager &events, Core::TimeDelta dt) override;
			void Update_Light();
		private:
			void Calculate_Light_CB_Size();
			void BakePixelShader();
			void BakeVertexShader();

			API::VertexShader VShader;
			API::PixelShader PShader;
			API::ConstantBuffer NE_Light_CB;

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
			API::Texture PostProcessTexture;
			API::RenderTarget PostProcessRT;
			API::VertexShader PostPrcoess_VShader;
			API::PixelShader PostPrcoess_PShader;
			XAsset::ScreenQuadAsset PostProcessScreenQuad;
		};

	}
}

//Implementation (Required since EntityX and C++ Templates doesn't play nicely with DLL Export)
#define RenderSystemImpl
#include "RenderSystemImpl.inl"
#undef RenderSystemImpl