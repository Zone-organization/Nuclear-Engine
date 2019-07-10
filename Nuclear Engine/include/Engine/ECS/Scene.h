#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>

namespace physx { class PxScene; };

namespace NuclearEngine
{
	namespace Components { class CameraComponent; }
	namespace ECS
	{
		class NEAPI Scene {
		public:
			Scene();
			~Scene();

			//Creates a new entity and assign a transform component to it automatically
			Entity CreateEntity();

			physx::PxScene* GetPhysXScene();
			void SetPhysXScene(physx::PxScene* scene);

			EventManager Events;
			EntityManager Entities;
			SystemManager Systems;

		private:
			physx::PxScene* mScene;
		};
	}
}