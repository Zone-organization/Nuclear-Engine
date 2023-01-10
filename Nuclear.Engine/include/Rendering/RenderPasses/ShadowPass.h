#pragma once
#include <Components\LightComponent.h>
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
			Diligent::TEXTURE_FORMAT mFormat = Diligent::TEX_FORMAT_D32_FLOAT;
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

			void DirLightShadowDepthPass(Components::LightComponent* dirlight, Uint32 RTindex);
			void SpotLightShadowDepthPass(Components::LightComponent* spotlight, Uint32 RTindex);
			void PointLightShadowDepthPass(Components::LightComponent* pointlight, Uint32 RTindex);

			ShadowPassBakingDesc GetBakingDesc() const;

			Diligent::IBuffer* GetLightSpacesCB();

			Diligent::ITextureView* GetDirPosShadowMapSRV();

			Diligent::ITextureView* GetSpotShadowMapSRV();

			Diligent::ITextureView* GetOmniDirShadowMapSRV();

		protected:
			ShadowPassBakingDesc mDesc;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pLightSpacesCB;

			//Spotlight/DirLight (simple)
			struct PosShadowMap
			{
				Diligent::RefCntAutoPtr<Diligent::ITexture> pPosShadowMap;
				Diligent::RefCntAutoPtr<Diligent::ITextureView> pPosShadowMapSRV;
				std::vector<Diligent::RefCntAutoPtr<Diligent::ITextureView>> pPosShadowMapDSVs;
			};

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mPositionalShadowMapDepthPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mPositionalShadowMapDepthSRB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pPositionalLightInfoCB;
			PosShadowMap mDirShadowMap;
			PosShadowMap mSpotShadowMap;

			void PositionalLightShadowDepthPass(Uint32 RTindex,const Math::Matrix4 lightspace, PosShadowMap& type);
			void InitPositionalShadowPassPSO();
			void InitPositionalShadowMapTextures();

			//Pointlight
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mOmniDirShadowPassPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mOmniDirShadowPassSRB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pOmniDirShadowVS_CB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pOmniDirShadowGS_CB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pOmniDirShadowPS_CB;
			Diligent::RefCntAutoPtr<Diligent::ITexture> pOmniDirShadowMap;
			Diligent::RefCntAutoPtr<Diligent::ITextureView> pOmniDirShadowMapSRV;
			std::vector<Diligent::RefCntAutoPtr<Diligent::ITextureView>> pOmniDirShadowMapDSVs;

			void InitOmniDirShadowPassPSO();
			void InitOmniDirShadowMapTexture();

			void RenderMeshForDepthPass(Assets::Mesh* mesh);
		};

	}
}