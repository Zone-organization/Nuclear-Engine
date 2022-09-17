#pragma once
#include <Core\NE_Common.h>
#include <Engine\Assets\Scene.h>
#include <vector>
#include <string>
#include <Engine\ECS\Entity.h>

namespace Nuclear
{
	namespace Managers
	{
		//Basically the real 'active-scene' which holds entt registry
		class NEAPI SceneManager
		{
		public:
			SceneManager();
			~SceneManager();

			void CreateScene(Assets::Scene* scene, bool SetAsActive = false);

			ECS::SystemManager& GetSystemManager();
			entt::registry& GetRegistry();
			void Update(ECS::TimeDelta dt);                  //Only update active scene

			void SetActiveScene(Assets::Scene* scene);
			Assets::Scene* GetActiveScene();

		protected:
			entt::registry Registry;
			ECS::SystemManager Systems;

			Assets::Scene* mActiveScene;
		};
	}
}