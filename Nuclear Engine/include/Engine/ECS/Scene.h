#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Graphics\Background.h>

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