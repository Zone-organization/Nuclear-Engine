#include <Engine\ECS\Scene.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <Engine\Components\EntityInfoComponent.h>

namespace Nuclear
{
	namespace ECS
	{
		Scene::Scene(const std::string& name)
			: mName(name), Factory(this), Systems(this)
		{

		}
		Scene::~Scene()
		{
		//	Entities.reset();
		}
		Entity Scene::CreateEntity()
		{
			Entity result;
			result.entity = Registry.create();
			result.parent = &Registry;
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>();
			einfo.mOwnerEntity = result;

			return result;
		}
		Entity Scene::CreateEntity(const std::string& name, const ECS::Transform& transform)
		{
			Entity result;
			result.entity = Registry.create();
			result.parent = &Registry;
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>(transform, name);
			einfo.mOwnerEntity = result;

			return result;
		}
		std::string Scene::GetName()
		{
			return mName;
		}

		void Scene::Update(ECS::TimeDelta dt)
		{
			Systems.Update_All(dt);
		}

		SystemManager& Scene::GetSystemManager()
		{
			return Systems;
		}

		entt::registry& Scene::GetRegistry()
		{
			return Registry;
		}

		EntityFactory& Scene::GetFactory()
		{
			return Factory;
		}

		void Scene::Save()
		{
			std::stringstream storage;

			entt::registry destination;

			{
				// output finishes flushing its contents when it goes out of scope
				cereal::JSONOutputArchive output{ storage };
				entt::snapshot{ Registry }.entities(output).component<Components::EntityInfoComponent>(output);
			}

			cereal::JSONInputArchive input{ storage };
			entt::snapshot_loader{ destination }.entities(input).component<Components::EntityInfoComponent>(input);
		}
	}
}