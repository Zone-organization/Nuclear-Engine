#pragma once
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine/Assets/Material.h>
#include <Engine\PhysX\PhysXMaterial.h>
#include <Engine\ECS\Transform.h>

namespace NuclearEngine
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


			Entity CreateBox(Assets::Material* material, ECS::Transform t , PhysX::PhysXMaterial* PMat = GetDefaultBoxMaterial());

			Entity CreateSphere(Assets::Material* material, ECS::Transform t, PhysX::PhysXMaterial* PMat = GetDefaultSphereMaterial());

			Entity CreatePlane(Assets::Material* material, ECS::Transform t, PhysX::PhysXMaterial* PMat = GetDefaultPlaneMaterial());

			static PhysX::PhysXMaterial* GetDefaultBoxMaterial();
			static PhysX::PhysXMaterial* GetDefaultSphereMaterial();
			static PhysX::PhysXMaterial* GetDefaultPlaneMaterial();
			static void InitializeDefaultPhysxMaterials();
		protected:

			ECS::Scene* mScene;
		};
	}
}