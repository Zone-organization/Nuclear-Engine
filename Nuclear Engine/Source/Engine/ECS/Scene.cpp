#include <Engine\ECS\Scene.h>
#include <Engine\ECS\ComponentDependency.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include<Core\Parsers\XMLParser.h>

namespace NuclearEngine
{
	namespace ECS
	{
		Scene::Scene()
			: Entities(Events),Systems(Entities, Events), mScene(nullptr), Factory(this)
		{
		}
		Scene::~Scene()
		{
		}
		Entity Scene::CreateEntity()
		{
			auto entity = Entities.Create();
			entity.Assign<Components::EntityInfoComponent>();
			entity.GetComponent<Components::EntityInfoComponent>().Get()->mOwnerEntity = entity;
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
		SceneLoader::SceneLoader()
		{
		}
		SceneLoader::~SceneLoader()
		{
		}
		Scene* SceneLoader::LoadScene(Core::Path path)
		{
			using namespace Core::Parsers;
			XMLDocument SceneDoc;
			SceneDoc.LoadFile(path.mRealPath.c_str());

			XMLElement* root = SceneDoc.FirstChildElement();
			//root.

			return nullptr;
		}
	}
}