#pragma once
#include <Core\NE_Common.h>
#include <Engine/ECS/System.h>
#include <Core\Path.h>
#include <Engine/Assets/Common.h>
#include <Engine\ECS\Entity.h>
#include <Engine\ECS\Transform.h>

namespace Nuclear
{
	namespace Managers {
		class SceneManager;
	}
	namespace Components { class CameraComponent; }
	namespace Assets
	{
		class Material;

		class NEAPI Scene : public Asset<Scene>
		{
			friend class Managers::SceneManager;
		public:
			Scene();
			~Scene();

			//Creates a new entity and assign a transform component to it automatically
			ECS::Entity CreateEntity();
			ECS::Entity CreateEntity(const std::string& name, const ECS::Transform& transform = ECS::Transform());

			ECS::Entity CreateBox(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);
			ECS::Entity CreateSphere(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);
			ECS::Entity CreatePlane(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);

			//void Save();
			ECS::SystemManager& GetSystemManager();
			entt::registry& GetRegistry();

		private: 
			Managers::SceneManager* ParentSceneMgr;
		};
	}
}