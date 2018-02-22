#pragma once
#include <NE_Common.h>
#include <Core/Entity.h>
#include "Core/Event.h"
#include "Core/System.h"
#include <Components\GameObject.h>
#include <Shading\Technique.h>
#include <Components\GenericCamera.h>
#include <Components\Light.h>
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
			Shading::Technique* Light_Rendering_Tech = nullptr;

		};

		class NEAPI RenderSystem : public Core::System<RenderSystem> {
		public:
			RenderSystem(const RenderSystemDesc& desc = RenderSystemDesc());
			~RenderSystem();

			void SetCamera(Components::GenericCamera* camera);

			API::Shader GetShader();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void Bake();

			void Render_Light();

			//Updating the render system is rendering it
			void Update(Core::EntityManager &es, Core::EventManager &events, Core::TimeDelta dt) override;
		private:
			void Calculate_Light_CB_Size();

			API::Shader Shader;
			API::ConstantBuffer NE_Light_CB;
			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;
			Shading::Technique* Light_Rendering_Tech;
			Components::GenericCamera* Camera;
			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;
			RenderSystemStatus status;
		};

	}
}