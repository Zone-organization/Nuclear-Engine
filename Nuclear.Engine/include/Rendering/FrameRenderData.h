#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics/RenderTarget.h>
#include <Components/LightComponent.h>
#include <vector>
#include <map>
#include <ECS/Entity.h>
#include <Components/MeshComponent.h>

namespace Nuclear
{
    namespace Systems {
        class CameraSystem;
    }
    namespace Assets {
        class Mesh;
        class Material;
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
            MeshView mMeshView;

            RefCntAutoPtr<IBuffer> pAnimationCB;

            //baked lights
            std::vector<Components::LightComponent*> DirLights;
            std::vector<Components::LightComponent*> PointLights;
            std::vector<Components::LightComponent*> SpotLights;

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