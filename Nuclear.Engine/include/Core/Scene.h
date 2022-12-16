#pragma once
#include <NE_Common.h>
#include <ECS/System.h>
#include <Core\Path.h>
#include <Assets/Common.h>
#include <ECS\Entity.h>
#include <ECS\Transform.h>
#include <Interfaces\Singleton.h>
#include <Assets/SavedScene.h>

namespace Nuclear
{
	namespace Components { class CameraComponent; }
	namespace Assets { class Material; }

	namespace Core
	{
		class NEAPI Scene : public Interfaces::Singleton<Scene>
		{
		public:
			//Creates a new entity and assign a transform component to it automatically
			ECS::Entity CreateEntity();
			ECS::Entity CreateEntity(const std::string& name, const ECS::Transform& transform = ECS::Transform());

			ECS::Entity CreateBox(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);
			ECS::Entity CreateSphere(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);
			ECS::Entity CreatePlane(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);

			ECS::SystemManager& GetSystemManager();
			entt::registry& GetRegistry();

			void Update(ECS::TimeDelta dt);

			//Serialization
			bool SaveScene(Assets::SavedScene* scene);

			bool LoadScene(Assets::SavedScene* scene);


		private:
			entt::registry mRegistry;
			ECS::SystemManager mSystems;
		};
	}
}