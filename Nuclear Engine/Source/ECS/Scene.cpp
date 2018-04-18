#include <ECS\Scene.h>
#include <ECS\ComponentDependency.h>
#include <Components\MeshComponent.h>
#include <Components\Skybox.h>

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
	}
}