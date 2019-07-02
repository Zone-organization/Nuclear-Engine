#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <vector>
#include <unordered_map>

namespace NuclearEngine
{

	namespace Systems
	{


		class NEAPI CameraSystem : public ECS::System<CameraSystem> {
		public:
			CameraSystem(Components::CameraComponent* camera = nullptr);
			~CameraSystem();


			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;


		private:

		};

	}
}