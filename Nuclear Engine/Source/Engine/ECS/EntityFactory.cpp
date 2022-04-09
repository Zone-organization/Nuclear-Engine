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

		EntityFactory::EntityFactory(ECS::Scene* ParentScene)
			: mScene(ParentScene)
		{
		}
		Entity EntityFactory::CreateBox(Assets::Material* material, ECS::Transform t)
		{
			Entity Result = mScene->CreateEntity("Box");

			Result.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), material);
			Result.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(t.GetTransform());

			auto System = mScene->GetSystemManager().GetSystem<Systems::PhysXSystem>();

			if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
			{
				PhysX::BoxGeometry box(Math::Vector3(0.5f, 0.5f, 0.5f), t);
				Components::ColliderDesc desc;
				desc.mGeo = &box;
				desc.mShape = Components::ColliderShape::Box;
				Result.AddComponent<Components::ColliderComponent>(desc);
				Result.AddComponent<Components::RigidBodyComponent>(t);
				System->AddActor(Result);
				System->SetColliderForRigidBody(Result);
				Result.GetComponent<Components::ColliderComponent>()->SetValid(true);
			}
			return Result;
		}
		Entity EntityFactory::CreateSphere(Assets::Material* material, ECS::Transform t)
		{
			Entity Result = mScene->CreateEntity("Sphere");
			Result.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), material);
			Result.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(t.GetTransform());

			auto System = mScene->GetSystemManager().GetSystem<Systems::PhysXSystem>();

			if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
			{
				PhysX::SphereGeometry sphere(0.5f, t);
				Components::ColliderDesc desc;
				desc.mGeo = &sphere;
				desc.mShape = Components::ColliderShape::Sphere;
				Result.AddComponent<Components::ColliderComponent>(desc);
				Result.AddComponent<Components::RigidBodyComponent>(t);
				System->AddActor(Result);
				System->SetColliderForRigidBody(Result);
				Result.GetComponent<Components::ColliderComponent>()->SetValid(true);
			}

			return Result;
		}
		Entity EntityFactory::CreatePlane(Assets::Material* material, ECS::Transform t)
		{
			return Entity();
		}
	}
}