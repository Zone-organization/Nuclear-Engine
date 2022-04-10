#pragma once
#include <Core\NE_Common.h>
#include <Engine/ECS/EntityFactory.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/System.h>
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

			void Update(ECS::TimeDelta dt);

			SystemManager& GetSystemManager();
			entt::registry& GetRegistry();
			EntityFactory& GetFactory();

		private:
			std::string mName;
			EntityFactory Factory;
			entt::registry Registry;
			SystemManager Systems;

		};

		class NEAPI SceneLoader {
		public:
			SceneLoader();
			~SceneLoader();

			Scene* LoadScene(Core::Path path);
		};


	}
}