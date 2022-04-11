#include <Engine\ECS\Scene.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include<Core\Parsers\XMLParser.h>

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
		Entity Scene::CreateEntity(const char* name)
		{
			Entity result;
			result.entity = Registry.create();
			result.parent = &Registry;
			auto &einfo = result.AddComponent<Components::EntityInfoComponent>();
			einfo.mOwnerEntity = result;
			einfo.mName = name;

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