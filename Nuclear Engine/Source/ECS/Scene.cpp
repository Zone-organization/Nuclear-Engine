#include <ECS\Scene.h>
#include <ECS\ComponentDependency.h>
#include <Components\Model.h>
#include <Components\Skybox.h>
#include <Components\ModelRenderDesc.h>

namespace NuclearEngine
{
	namespace ECS
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events)
		{
			Systems.Add<Dependency<Components::Model, Components::ModelRenderDesc>>();
			Systems.Add<Dependency<Components::Skybox, Components::ModelRenderDesc>>();

		}
		Scene::~Scene()
		{
		}
	}
}