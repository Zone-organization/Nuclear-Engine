#pragma once
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Rendering\ShadingModel.h>
#include <vector>
#include <Engine/Rendering/BasicShadowMapManager.h>

namespace Nuclear
{
	namespace Systems
	{
		class NEAPI LightingSubSystem 
		{
		public:
			std::vector<Components::DirLightComponent*> DirLights;
			std::vector<Components::PointLightComponent*> PointLights;
			std::vector<Components::SpotLightComponent*> SpotLights;

			bool RequiresBaking();

			void Initialize();

			void BakeBuffer();

			//Update Functions
			void UpdateBuffer(const Math::Vector4& CameraPos);

			//Debug Only
			bool VisualizePointLightsPositions = false;

			RefCntAutoPtr<IBuffer> mPSLightCB;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

		private:
			size_t Baked_DirLights_Size = 0;
			size_t Baked_PointLights_Size = 0;
			size_t Baked_SpotLights_Size = 0;

			bool HasbeenBakedBefore = false;

			Rendering::BasicShadowMapManager SpotlightsShadowManager;
		};

	}
}