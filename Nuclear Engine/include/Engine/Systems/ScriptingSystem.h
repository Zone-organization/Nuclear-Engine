#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>

namespace NuclearEngine
{
	namespace Systems
	{
		class NEAPI ScriptingSystem : public ECS::System<ScriptingSystem> {
		public:
			ScriptingSystem();
			~ScriptingSystem();


			
			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:

		};

	}
}