#pragma once
#include <Engine\ECS/Common.h>
#include <Engine\ECS\Entity.h>
#include <Engine/Assets/Material.h>
#include <Engine\ECS\Transform.h>

namespace Nuclear
{
	namespace Systems { 
		class PhysXSystem;
	}
	namespace ECS
	{
		class Scene;
		class NEAPI EntityFactory 
		{
		public:
			EntityFactory(ECS::Scene* ParentScene);

			Entity CreateBox(Assets::Material* material,const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);

			Entity CreateSphere(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);

			Entity CreatePlane(Assets::Material* material, const ECS::Transform& transform = ECS::Transform(), bool addcollider = true);

		protected:

			ECS::Scene* mScene;
		};
	}
}