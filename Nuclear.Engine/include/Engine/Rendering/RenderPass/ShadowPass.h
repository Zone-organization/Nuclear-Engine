#pragma once
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Rendering\ShadingModel.h>
#include <vector>
#include <Engine\ECS\System.h>

namespace Nuclear
{
	namespace Rendering
	{

		class CameraSystem;
		struct ShadowMapInfo
		{
			Uint32 mResolution = 1024;
			TEXTURE_FORMAT mFormat = TEX_FORMAT_D32_FLOAT;
		};

		struct ShadowPassDesc
		{
			//max limits used for compiling shaders... (to avoid recompiling pipeline shaders everytime a new light is added to the scene or cast shadows activated)
			//Note: has no effect on shadow pass pipelines. + used in pVSShadowCasterBuffer

			Uint32 MAX_DIR_CSM_CASTERS = 1;   //WIP

			Uint32 MAX_DIR_CASTERS = 1;
			Uint32 MAX_SPOT_CASTERS = 1;

			Uint32 MAX_POS_CASTERS = 1;
			Uint32 MAX_OMNIDIR_CASTERS = 1;

		//	ShadowMapInfo mDirLightShadowMapInfo;
			ShadowMapInfo mPositionalShadowMapInfo;  //aka spot light shadows / simple (not csm) dir light shadow maps
			ShadowMapInfo mOmniDirShadowMapInfo;     //Aka point light shadows

		};
		class NEAPI ShadowPass
		{
		public:
			ShadowPass(const ShadowPassDesc& desc = ShadowPassDesc());

			void Initialize();

			void DirLightShadowDepthPass(Components::DirLightComponent& dirlight, Assets::Scene* scene);
			void SpotLightShadowDepthPass(Components::SpotLightComponent& spotlight, Assets::Scene* scene);
			void PointLightShadowDepthPass(Components::PointLightComponent& pointlight, Assets::Scene* scene);

			ShadowPassDesc GetDesc() const;

			IBuffer* GetShadowCastersCB();
		protected:
			ShadowPassDesc mDesc;
			RefCntAutoPtr<IBuffer> pVSShadowCasterBuffer;

			//Spotlight/DirLight (simple)
			RefCntAutoPtr<IPipelineState> mPositionalShadowMapDepthPSO;
			RefCntAutoPtr<IShaderResourceBinding> mPositionalShadowMapDepthSRB;
			RefCntAutoPtr<IBuffer> pPositionalLightInfoCB;
			RefCntAutoPtr<ITexture> pPosShadowMap;
			RefCntAutoPtr<ITextureView> pPosShadowMapSRV;

			void PositionalLightShadowDepthPass(Graphics::ShadowMap* shadowmap, const Math::Matrix4 lightspace, Assets::Scene* scene);
			void InitPositionalShadowPassPSO();
			void InitPositionalShadowMapTexture();

			//Pointlight
			RefCntAutoPtr<IPipelineState> mOmniDirShadowPassPSO;
			RefCntAutoPtr<IShaderResourceBinding> mOmniDirShadowPassSRB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowVS_CB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowGS_CB;
			RefCntAutoPtr<IBuffer> pOmniDirShadowPS_CB;
			void InitOmniDirShadowPassPSO();
		
			void RenderMeshForDepthPass(Assets::Mesh* mesh);
		};

	}
}