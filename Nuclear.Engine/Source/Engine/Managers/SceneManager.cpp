#include "Engine/Managers/SceneManager.h"

namespace Nuclear
{
	namespace Managers
	{
		SceneManager::SceneManager()
		{
		}
		SceneManager::~SceneManager()
		{
		}
		ECS::SystemManager& SceneManager::GetSystemManager()
		{
			return Systems;
		}
		entt::registry& SceneManager::GetRegistry()
		{
			return Registry;
		}
		void SceneManager::SetActiveScene(Assets::Scene* scene)
		{
			mActiveScene = scene;
		}
		Assets::Scene* SceneManager::GetActiveScene()
		{
			return mActiveScene;
		}
	}
}