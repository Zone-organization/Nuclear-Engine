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
        class ShaderPipelineVariant;
    }
	namespace Rendering
	{
        typedef entt::basic_view<entt::entity, entt::get_t<Nuclear::Components::MeshComponent>, entt::exclude_t<>, void> MeshView;

		struct NEAPI FrameRenderData
		{
		public:
            MeshView mMeshView;

            //baked lights
            std::vector<Components::LightComponent*> DirLights;
            std::vector<Components::LightComponent*> PointLights;
            std::vector<Components::LightComponent*> SpotLights;

            std::vector<Graphics::ShaderPipelineVariant*> mUsedDefferedPipelines;

            //shadows
            bool mShadowsEnabled;
            RefCntAutoPtr<ITextureView> pDirPosShadowMapSRV;
            RefCntAutoPtr<ITextureView> pSpotPosShadowMapSRV;
            RefCntAutoPtr<ITextureView> pOmniDirShadowMapSRV;

            //Rendered Scene here          
            Components::CameraComponent* pCamera;
		};
	}
}