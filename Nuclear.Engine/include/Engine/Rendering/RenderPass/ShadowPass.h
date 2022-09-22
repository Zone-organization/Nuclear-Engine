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
			Uint32 MAX_DIR_CASTERS = 1;
			Uint32 MAX_SPOT_CASTERS = 0;
			//Uint32 MAX_POINT_CASTERS = 1;



			ShadowMapInfo mDirLightShadowMapInfo;
			ShadowMapInfo mSpotLightShadowMapInfo;
			ShadowMapInfo mPointLightShadowMapInfo;

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

			//DirLight (simple)
			RefCntAutoPtr<IPipelineState> mDirShadowMapDepthPSO;
			RefCntAutoPtr<IShaderResourceBinding> mDirShadowMapDepthSRB;
			RefCntAutoPtr<IBuffer> pDirLightInfoCB;

			//Spotlight
			RefCntAutoPtr<IPipelineState> mSpotShadowMapDepthPSO;
			RefCntAutoPtr<IShaderResourceBinding> mSpotShadowMapDepthSRB;
			RefCntAutoPtr<IBuffer> pSpotLightInfoCB;

			//Pointlight
			RefCntAutoPtr<IPipelineState> mPointShadowPassPSO;
			RefCntAutoPtr<IShaderResourceBinding> mPointShadowPassSRB;
			RefCntAutoPtr<IBuffer> pPointShadowVS_CB;
			RefCntAutoPtr<IBuffer> pPointShadowGS_CB;
			RefCntAutoPtr<IBuffer> pPointShadowPS_CB;

			void InitDirLightSimpleShadowPassPSO();

			void InitSpotLightShadowPSO();
			void InitPointLightShadowPassPSO();
		
			void RenderMeshForDepthPass(Assets::Mesh* mesh);
		};

	}
}