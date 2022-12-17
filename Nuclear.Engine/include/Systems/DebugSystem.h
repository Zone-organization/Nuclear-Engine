#pragma once
#include <ECS\System.h>
#include <Components\MeshComponent.h>
#include <Assets/Mesh.h>
#include <Assets/Material.h>
#include <Rendering\Background.h>
#include <vector>
#include <unordered_map>
#include <Graphics\ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Graphics {
		class RenderTarget;
	}
	namespace Systems
	{
		class NEAPI DebugSystem : public ECS::System<DebugSystem>
		{
		public:
			DebugSystem();
			~DebugSystem();

			//Update Functions
			void Update(ECS::TimeDelta dt) override;

			void ShowRendertargets();
			void AddRenderTarget(Graphics::RenderTarget* rt);

			std::vector<Graphics::RenderTarget*> mRegisteredRTs;
			bool RenderLightSources = true;

			bool ShowRegisteredRenderTargets = false;

			void InstantRender(Assets::Mesh* mesh, Assets::Image* diffusetex);

		protected:
			Graphics::ShaderPipelineVariant pShader;
		};

	}
}