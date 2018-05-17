#include <Engine\ECS\Scene.h>
#include <Engine\ECS\ComponentDependency.h>
#include <Engine\Components\MeshComponent.h>

namespace NuclearEngine
{
	namespace ECS
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events), ActiveCamera(nullptr)
		{
		}
		Scene::~Scene()
		{
		}
		void Scene::SetActiveCamera(Components::CameraComponent * camera)
		{
			ActiveCamera = camera;
		}
		Components::CameraComponent * Scene::GetActiveCamera()
		{
			return ActiveCamera;
		}
	}
}