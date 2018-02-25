#include <Core\Scene.h>
#include <Core\ComponentDependency.h>
#include <Components\Model.h>
#include <Components\Transform.h>

namespace NuclearEngine
{
	namespace Core
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events)
		{
			Systems.Add<Dependency<Components::Model, Components::Transform>>();
		}
		Scene::~Scene()
		{
		}
	}
}