#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine/ECS/EntityFactory.h>
#include <Core\Path.h>

namespace NuclearEngine
{
	namespace Components { class CameraComponent; }
	namespace ECS
	{
		class NEAPI Scene {
		public:
			Scene(const std::string& name = "scene");
			~Scene();

			//Creates a new entity and assign a transform component to it automatically
			Entity CreateEntity();
			Entity CreateEntity(const char* name);

			std::string GetName();

			EventManager Events;
			EntityManager Entities;
			SystemManager Systems;
			EntityFactory Factory;
		private:
			std::string mName;
		};

		class NEAPI SceneLoader {
		public:
			SceneLoader();
			~SceneLoader();

			Scene* LoadScene(Core::Path path);
		};


	}
}