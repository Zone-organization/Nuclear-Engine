#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>

namespace NuclearEngine
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
		};

	}
}