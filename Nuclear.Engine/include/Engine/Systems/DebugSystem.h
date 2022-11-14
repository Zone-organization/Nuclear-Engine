#pragma once
#include <Engine\ECS\System.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <Engine\Rendering\Background.h>
#include <vector>
#include <unordered_map>
#include <Engine\Rendering\RenderingPaths\ForwardRenderingPath.h>
#include <Engine\Rendering\ShadingModels\DiffuseOnly.h>

namespace Nuclear
{
	namespace Graphics {
		class Camera;
	}
	namespace Systems
	{
		class CameraSystem;

		class NEAPI DebugSystem : public ECS::System<DebugSystem>
		{
		public:
			DebugSystem();
			~DebugSystem();

			void Initialize(Graphics::Camera* camera, IBuffer* AnimationBufferPtr );

			//Update Functions
			void Update(ECS::TimeDelta dt) override;



			void ShowRendertargets();
			void AddRenderTarget(Graphics::RenderTarget* rt);

			std::vector<Graphics::RenderTarget*> mRegisteredRTs;
			bool RenderLightSources = true;

			bool ShowRegisteredRenderTargets = false;

			void InstantRender(Assets::Mesh* mesh, Assets::Image* diffusetex);

		protected:
			Rendering::DiffuseOnly DebugRP;
			RefCntAutoPtr<IShaderResourceBinding> mPipelineSRB;
		};

	}
}