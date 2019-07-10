#include <Engine\ECS\Scene.h>
#include <Engine\ECS\ComponentDependency.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\TransformComponent.h>

namespace NuclearEngine
{
	namespace ECS
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events)
		{
		}
		Scene::~Scene()
		{
		}
		Entity Scene::CreateEntity()
		{
			auto entity = Entities.Create();
			entity.Assign<Components::TransformComponent>();
			return entity;
		}
		void Scene::SetPhysXScene(physx::PxScene* scene)
		{
			mScene = scene;
		}
		physx::PxScene* Scene::GetPhysXScene()
		{
			return mScene;
		}
	}
}