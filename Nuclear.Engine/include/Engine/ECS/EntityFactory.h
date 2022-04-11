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

			Entity CreateBox(Assets::Material* material, ECS::Transform t);

			Entity CreateSphere(Assets::Material* material, ECS::Transform t);

			Entity CreatePlane(Assets::Material* material, ECS::Transform t);

		protected:

			ECS::Scene* mScene;
		};
	}
}