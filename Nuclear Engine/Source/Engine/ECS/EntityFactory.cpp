#include <Engine\ECS\EntityFactory.h>
#include <Engine\ECS\Scene.h>
#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine/Assets/DefaultMeshes.h>
#include "..\PhysX\PhysXTypes.h"

namespace NuclearEngine
{
	namespace ECS
	{
		static PhysX::PhysXMaterial gDefaultBoxMaterial;
		static PhysX::PhysXMaterial gDefaultSphereMaterial;
		static PhysX::PhysXMaterial gDefaultPlaneMaterial;

		EntityFactory::EntityFactory(ECS::Scene* ParentScene)
			: mScene(ParentScene)
		{
		}
		Entity EntityFactory::CreateBox(Assets::Material* material, ECS::Transform t, PhysX::PhysXMaterial* PMat)
		{
			Entity Result = mScene->CreateEntity("Box");
			Result.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), material);
			Result.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(t.GetTransform());


			auto System = mScene->Systems.GetSystem<Systems::PhysXSystem>();
			if (PMat != nullptr)
			{
				if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
				{
					Result.Assign<Components::ColliderComponent>(PMat, PhysX::BoxGeometry(Math::Vector3(0.5f, 0.5f, 0.5f), t));
					Result.Assign<Components::RigidBodyComponent>(t);
					System->AddActor(Result);
					System->SetColliderForRigidBody(Result);
					Result.GetComponent<Components::ColliderComponent>()->mAddedtoPhysxScene = true;
				}
			}
			return Result;
		}
		Entity EntityFactory::CreateSphere(Assets::Material* material, ECS::Transform t, PhysX::PhysXMaterial* PMat)
		{
			Entity Result = mScene->CreateEntity("Sphere");
			Result.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), material);
			Result.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(t.GetTransform());

			auto System = mScene->Systems.GetSystem<Systems::PhysXSystem>();
			if (PMat != nullptr)
			{
				if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
				{
					Result.Assign<Components::ColliderComponent>(PMat, PhysX::SphereGeometry(0.5f, t));
					Result.Assign<Components::RigidBodyComponent>(t);
					System->AddActor(Result);
					System->SetColliderForRigidBody(Result);
					Result.GetComponent<Components::ColliderComponent>()->mAddedtoPhysxScene = true;
				}
			}
			return Result;
		}
		Entity EntityFactory::CreatePlane(Assets::Material* material, ECS::Transform t, PhysX::PhysXMaterial* PMat)
		{
			return Entity();
		}
		PhysX::PhysXMaterial* EntityFactory::GetDefaultBoxMaterial()
		{
			return &gDefaultBoxMaterial;
		}
		PhysX::PhysXMaterial* EntityFactory::GetDefaultSphereMaterial()
		{
			return &gDefaultSphereMaterial;
		}
		PhysX::PhysXMaterial* EntityFactory::GetDefaultPlaneMaterial()
		{
			return &gDefaultPlaneMaterial;
		}
		void EntityFactory::InitializeDefaultPhysxMaterials()
		{
			gDefaultBoxMaterial.Create();
			gDefaultSphereMaterial.Create();
			gDefaultPlaneMaterial.Create();
		}
	}
}