#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\Light.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
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
			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;

			void BakeBuffer();

			//Update Functions
			void UpdateBuffer(const Math::Vector4& CameraPos);

			//Debug Only
			bool VisualizePointLightsPositions = false;

			RefCntAutoPtr<IBuffer> mPSLightCB;

			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;
		};

	}
}