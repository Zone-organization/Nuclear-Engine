#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Components/LightComponent.h>
#include <vector>
#include <map>
#include <ECS/Entity.h>
#include <Components/MeshComponent.h>
#include <Components/CameraComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>

namespace Nuclear
{
    namespace Assets {
        class Mesh;
        class Material;
        class Shader;
    }
    namespace Graphics {
        struct ShaderPipelineVariant;
    }
    namespace Systems
    {
        class RenderSystem;
    }
	namespace Rendering
	{
        typedef entt::basic_view<entt::entity, entt::get_t<Nuclear::Components::MeshComponent>, entt::exclude_t<>, void> MeshView;

		struct FrameRenderData
		{
            Systems::RenderSystem* pRenderer = nullptr;

            MeshView mMeshView;

            //baked lights
            std::vector<Components::LightComponent*> DirLights;
            std::vector<Components::LightComponent*> PointLights;
            std::vector<Components::LightComponent*> SpotLights;

            std::vector<Graphics::ShaderPipelineVariant*> mUsedDefferedPipelines;

            //shadows
            bool mShadowsEnabled;
            Diligent::RefCntAutoPtr<Diligent::ITextureView> pDirPosShadowMapSRV;
            Diligent::RefCntAutoPtr<Diligent::ITextureView> pSpotPosShadowMapSRV;
            Diligent::RefCntAutoPtr<Diligent::ITextureView> pOmniDirShadowMapSRV;

            //Rendered Scene here          
            Components::CameraComponent* pCamera = nullptr;
		};
	}
}