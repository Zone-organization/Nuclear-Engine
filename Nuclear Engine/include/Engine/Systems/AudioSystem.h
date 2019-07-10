#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>

namespace NuclearEngine
{
	namespace Systems
	{
		class NEAPI AudioSystem : public ECS::System<AudioSystem> {
		public:
			AudioSystem();
			~AudioSystem();

	

			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:

		};

	}
}