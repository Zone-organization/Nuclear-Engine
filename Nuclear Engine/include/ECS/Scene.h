#pragma once
#include <NE_Common.h>
#include <ECS/Entity.h>
#include "ECS/Event.h"
#include "ECS/System.h"

namespace NuclearEngine
{
	namespace Components { class CameraComponent; }
	namespace ECS
	{
		class NEAPI Scene {
		public:
			Scene();
			~Scene();

			void SetActiveCamera(Components::CameraComponent* camera);
			Components::CameraComponent* GetActiveCamera();

			EventManager Events;
			EntityManager Entities;
			SystemManager Systems;

		private:
			Components::CameraComponent *ActiveCamera;
		};
	}
}