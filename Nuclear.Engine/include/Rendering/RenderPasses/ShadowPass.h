#pragma once
#include <Components\DirLightComponent.h>
#include <Components\PointLightComponent.h>
#include <Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Rendering\RenderPass.h>
#include <vector>
#include <ECS\System.h>

namespace Nuclear
{
	namespace Assets {
		class Mesh;
	}
	namespace Rendering
	{
		class CameraSystem;
		struct ShadowMapInfo
		{
			Uint32 mResolution = 1024;
			TEXTURE_FORMAT mFormat = TEX_FORMAT_D32_FLOAT;
		};


		struct ShadowPassBakingDesc
		{
			//max limits used for compiling shaders... (to avoid recompiling pipeline shaders everytime a new light is added to the scene or cast shadows activated)
			//Note: has no effect on shadow pass pipelines. + used in pVSShadowCasterBuffer

			Uint32 MAX_DIR_CSM_CASTERS = 1;   //WIP

			Uint32 MAX_DIR_CASTERS = 0;
			Uint32 MAX_SPOT_CASTERS = 0;
			Uint32 MAX_OMNIDIR_CASTERS = 0;

			ShadowMapInfo mDirPosShadowMapInfo;
			ShadowMapInfo mSpotShadowMapInfo;
			ShadowMapInfo mOmniDirShadowMapInfo;     //Aka point light shadows

		};
		class NEAPI ShadowPass : public RenderPass
		{
		public:
			ShadowPass();

			void Bake(const ShadowPassBakingDesc& desc = ShadowPassBakingDesc());

			void Update(FrameRenderData* framedata) override;

			void DirLightShadowDepthPass(Components::DirLightComponent* dirlight, Uint32 RTindex, Assets::Scene* scene);
			void SpotLightShadowDepthPass(Components::SpotLightComponent* spotlight, Uint32 RTindex, Assets::Scene* scene);
			void PointLightShadowDepthPass(Components::PointLightComponent* pointlight, Uint32 RTindex, Assets::Scene* scene);

			ShadowPassBakingDesc GetBakingDesc() const;

			IBuffer* GetLightSpacesCB();

			ITextureView* GetDirPosShadowMapSRV();

			ITextureView* GetSpotShadowMapSRV();

			ITextureView* GetOmniDirShadowMapSRV();

		protected:
			ShadowPassBakingDesc mDesc;
			RefCntAutoPtr<IBuffer> pLightSpacesCB;

			//Spotlight/DirLight (simple)
			struct PosShadowMap
			{
				RefCntAutoPtr<ITexture> pPosShadowMap;
				RefCntAutoPtr<ITextureView> pPosShadowMapSRV;
				std::vector<RefCntAutoPtr<ITextureView>> pPosShadowMapDSVs;
			};

			RefCntAutoPtr<IPipelineState> mPositionalShadowMapDepthPSO;
			RefCntAutoPtr<IShaderResourceBinding> mPositionalShadowMapDepthSRB;
			RefCntAutoPtr<IBuffer> pPositionalLightInfoCB;
			PosShadowMap mDirShadowMap;
			PosShadowMap mSpotShadowMap;

			void PositionalLightShadowDepthPass(Uint32 RTindex,const Math::Matrix4 lightspace, Assets::Scene* scene, PosShadowMap& type);
			void InitPositionalShadowPassPSO();
			void InitPositionalShadowMapTextures();

			//Pointlight
			RefCntAutoPtr<IPipelineState> mOmniDirShadowPassPSO;
			RefCntAutoPtr<IShaderResourceBinding> mOmniDirShadowPassSRB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowVS_CB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowGS_CB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowPS_CB;
			RefCntAutoPtr<ITexture> pOmniDirShadowMap;
			RefCntAutoPtr<ITextureView> pOmniDirShadowMapSRV;
			std::vector<RefCntAutoPtr<ITextureView>> pOmniDirShadowMapDSVs;

			void InitOmniDirShadowPassPSO();
			void InitOmniDirShadowMapTexture();

			void RenderMeshForDepthPass(Assets::Mesh* mesh);
		};

	}
}