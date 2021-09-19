#include <Engine\ECS\EntityFactory.h>
#include <Engine\ECS\Scene.h>
#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
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
		Entity EntityFactory::CreateBox(Systems::PhysXSystem* System, PhysX::PhysXMaterial PMat, ECS::Transform t, Assets::Material* material, bool EnablePhysics)
		{
			Entity Result = mScene->CreateEntity();
			Result.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), material);
			Result.Assign<Components::ColliderComponent>(PMat, PhysX::BoxGeometry(Math::Vector3(0.5f, 0.5f, 0.5f), t));
			Result.Assign<Components::RigidBodyComponent>(t);

			mScene->GetPhysXScene()->addActor(*Result.GetComponent<Components::RigidBodyComponent>()->mDynamicActor.mPtr);
			System->SetColliderForRigidBody(Result);

			return Result;
		}
		Entity EntityFactory::CreateSphere(Systems::PhysXSystem* System, PhysX::PhysXMaterial PMat, ECS::Transform t, Assets::Material* material, bool EnablePhysics)
		{
			Entity Result = mScene->CreateEntity();
			Result.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), material);
			Result.Assign<Components::ColliderComponent>(PMat, PhysX::SphereGeometry(0.5f, t));
			Result.Assign<Components::RigidBodyComponent>(t);

			mScene->GetPhysXScene()->addActor(*Result.GetComponent<Components::RigidBodyComponent>()->mDynamicActor.mPtr);
			System->SetColliderForRigidBody(Result);

			return Result;
		}
	}
}