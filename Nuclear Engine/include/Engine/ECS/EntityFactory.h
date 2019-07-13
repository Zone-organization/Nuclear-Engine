#pragma once
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine/Assets/Material.h>
#include <Engine\PhysX\PhysXMaterial.h>
#include <Engine\Components\TransformComponent.h>

namespace NuclearEngine
{
	namespace Systems { class PhysXSystem; }
	namespace ECS
	{
		class Scene;
		class NEAPI EntityFactory 
		{
		public:
			EntityFactory(ECS::Scene* ParentScene);


			Entity CreateBox(Systems::PhysXSystem* System, PhysX::PhysXMaterial PMat, Components::TransformComponent* t, Assets::Material* material, bool EnablePhysics = false);

		protected:
			ECS::Scene* mScene;
		};
	}
}