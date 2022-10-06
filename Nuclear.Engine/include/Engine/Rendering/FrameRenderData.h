#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine/Graphics/RenderTarget.h>
#include <vector>
#include <map>

namespace Nuclear
{
    namespace Systems {
        class CameraSystem;
    }
    namespace Assets {
        class Mesh;
        class Material;
    }
    namespace Graphics {
        class Camera;
    }
    namespace Components {
        class AnimatorComponent;
    }
	namespace Rendering
	{
        class ShadingModel;
        struct DrawCommand
        {
            DrawCommand(Assets::Mesh* mesh, Assets::Material* material, const  Math::Matrix4& transform)
                : pMesh(mesh), pMaterial(material), mWorldTransform(transform) {}

            Assets::Mesh* GetMesh() const { return pMesh; }
            Assets::Material* GetMaterial() const { return pMaterial; }
            Components::AnimatorComponent* GetAnimator() const { return pAnimator; }
            const Math::Matrix4& GetTransform() const { return mWorldTransform; }

        private:
            Assets::Mesh* pMesh;
            Assets::Material* pMaterial;
            Math::Matrix4 mWorldTransform;
            Components::AnimatorComponent* pAnimator = nullptr;  //TODO: MOVE! : we should have skinned and non skinned shading models for performance concerns
        };

        struct DrawQueue
        {
            ShadingModel* pShadingModel;
            std::vector<DrawCommand> mDrawCommands;
        };

		struct NEAPI FrameRenderData
		{
		public:
            //Scene being rendered in this frame
            Assets::Scene* pScene;


            std::map<Uint32, DrawQueue> mSubmittedDraws;

            RefCntAutoPtr<IBuffer> pAnimationCB;

            //baked lights
            std::vector<Components::DirLightComponent*> DirLights;
            std::vector<Components::PointLightComponent*> PointLights;
            std::vector<Components::SpotLightComponent*> SpotLights;

            std::shared_ptr<Systems::CameraSystem> pCameraSystemPtr;


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