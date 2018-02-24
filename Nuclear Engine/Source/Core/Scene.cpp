#include <Core\Scene.h>
#include <Core\ComponentDependency.h>
namespace NuclearEngine
{
	namespace Core
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events)
		{
			Systems.Add<Dependency<Physics>>();
		}
		Scene::~Scene()
		{
		}
	}
}