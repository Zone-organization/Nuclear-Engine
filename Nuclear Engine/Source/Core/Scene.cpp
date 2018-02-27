#include <Core\Scene.h>
#include <Core\ComponentDependency.h>
#include <Components\Model.h>
#include <Components\Transform.h>
#include <Components\Skybox.h>
#include <Components\ModelRenderDesc.h>

namespace NuclearEngine
{
	namespace Core
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events)
		{
			Systems.Add<Dependency<Components::Model, Components::Transform, Components::ModelRenderDesc>>();
			Systems.Add<Dependency<Components::Skybox, Components::ModelRenderDesc>>();

		}
		Scene::~Scene()
		{
		}
	}
}