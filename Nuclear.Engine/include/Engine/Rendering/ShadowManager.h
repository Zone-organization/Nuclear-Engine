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

		struct ShadowManagerDesc
		{
			Uint32 MAX_SPOT_CASTERS = 1;
			ShadowMapInfo mSpotLightShadowMapInfo;
			ShadowMapInfo mPointLightShadowMapInfo;

		};
		class NEAPI ShadowManager
		{
		public:
			ShadowManager(const ShadowManagerDesc& desc = ShadowManagerDesc());

			void Initialize();

			void SpotLightShadowDepthPass(Components::SpotLightComponent& spotlight, ECS::Scene* scene);
			void PointLightShadowDepthPass(Components::PointLightComponent& pointlight, ECS::Scene* scene);

			ShadowManagerDesc GetDesc() const;

		protected:
			ShadowManagerDesc mDesc;

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


			void InitSpotLightShadowPSO();
			void InitPointLightShadowPassPSO();
		
			void RenderMeshForDepthPass(Assets::Mesh* mesh);
		};

	}
}