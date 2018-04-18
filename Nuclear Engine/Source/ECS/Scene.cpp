#include <ECS\Scene.h>
#include <ECS\ComponentDependency.h>
#include <Components\MeshComponent.h>
#include <Components\Skybox.h>

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