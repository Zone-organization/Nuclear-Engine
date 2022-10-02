#pragma once
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Rendering\ShadingModel.h>
#include <Engine\Rendering\RenderPass.h>
#include <vector>
#include <Engine\ECS\System.h>
#include <Engine/Rendering/RenderingPipeline.h>

namespace Nuclear
{
	namespace Rendering
	{

		class NEAPI GeometryPass : public RenderPass
		{
		public:
			GeometryPass();


			void Initialize(RenderingPipeline* pipeline);

			void Update(FrameRenderData* framedata) override;


		protected:

		};

	}
}