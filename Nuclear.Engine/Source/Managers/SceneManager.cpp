#include "Managers/SceneManager.h"
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Managers
	{
		SceneManager::SceneManager()
		{
			mActiveScene = nullptr;
		}
		SceneManager::~SceneManager()
		{
		}
		void SceneManager::CreateScene(Assets::Scene* scene, bool SetAsActive)
		{
			scene->ParentSceneMgr = this;

			if (scene->GetName() == std::string())
			{
				NUCLEAR_WARN("[SceneManager] Creating a scene with no name, hence the scene has been given a name automatically.");
				static int scenes = 0;
				scene->SetName("UnNamed_Scene" + std::to_string(scenes));
				scenes++;
			}
			if (SetAsActive)
			{
				SetActiveScene(scene);
			}
		}
		ECS::SystemManager& SceneManager::GetSystemManager()
		{
			return Systems;
		}
		entt::registry& SceneManager::GetRegistry()
		{
			return Registry;
		}
		void SceneManager::Update(ECS::TimeDelta dt)
		{		
			Systems.Update_All(dt);
		}
		void SceneManager::SetActiveScene(Assets::Scene* scene)
		{
			//TODO
			Systems.SetScene(scene);
			mActiveScene = scene;
		}
		Assets::Scene* SceneManager::GetActiveScene()
		{
			return mActiveScene;
		}
	}
}