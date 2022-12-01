#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine/Graphics/RenderTarget.h>
#include <Engine/Components/DirLightComponent.h>
#include <Engine/Components/SpotLightComponent.h>
#include <Engine/Components/PointLightComponent.h>
#include <vector>
#include <map>
#include <Engine\ECS/Entity.h>
#include <Engine/Components/MeshComponent.h>

namespace Nuclear
{
    namespace Systems {
        class CameraSystem;
    }
    namespace Assets {
        class Mesh;
        class Material;
        class Scene;
        class Shader;
    }
    namespace Graphics {
        class Camera;
        class ShaderPipelineVariant;
    }
	namespace Rendering
	{
        typedef entt::basic_view<entt::entity, entt::get_t<Nuclear::Components::MeshComponent>, entt::exclude_t<>, void> MeshView;

		struct NEAPI FrameRenderData
		{
		public:
            //Scene being rendered in this frame
            Assets::Scene* pScene;

            MeshView mMeshView;

            RefCntAutoPtr<IBuffer> pAnimationCB;

            //baked lights
            std::vector<Components::DirLightComponent*> DirLights;
            std::vector<Components::PointLightComponent*> PointLights;
            std::vector<Components::SpotLightComponent*> SpotLights;

            std::shared_ptr<Systems::CameraSystem> pCameraSystemPtr;

            std::vector<Graphics::ShaderPipelineVariant*> mUsedDefferedPipelines;

            //shadows
            bool mShadowsEnabled;
            RefCntAutoPtr<ITextureView> pDirPosShadowMapSRV;
            RefCntAutoPtr<ITextureView> pSpotPosShadowMapSRV;
            RefCntAutoPtr<ITextureView> pOmniDirShadowMapSRV;

            //Rendered Scene here
            Graphics::RenderTarget mFinalRT;
            Graphics::RenderTarget mFinalDepthRT;
            Graphics::Camera* pCamera;
		};
	}
}