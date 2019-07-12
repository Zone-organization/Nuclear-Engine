#include <Engine\ECS\EntityFactory.h>
#include <Engine\ECS\Scene.h>
#include <Engine\Systems\PhysXSystem.h>
#include <Engine\Components\ColliderComponent.h>
#include <Engine\Components\RigidBodyComponent.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine/Assets/DefaultMeshes.h>

namespace NuclearEngine
{
	namespace ECS
	{
		EntityFactory::EntityFactory(ECS::Scene* ParentScene)
			: mScene(ParentScene)
		{
		}
		Entity EntityFactory::CreateBox(Systems::PhysXSystem* System, PhysX::PhysXMaterial PMat, Assets::Material* material, bool EnablePhysics)
		{
			Entity Result = mScene->CreateEntity();
			Result.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), material);
			Result.Assign<Components::ColliderComponent>(PMat);
			Result.Assign<Components::RigidBodyComponent>();

			System->CreateBoxCollider(Result.GetComponent<Components::ColliderComponent>().Get(), PhysX::PxTransform(PhysX::PxVec3(0.0f, 3.0f, 0.0f)), PhysX::PxBoxGeometry(0.5f, 0.5f, 0.5f));
			System->CreateRigidBody(Result.GetComponent<Components::RigidBodyComponent>().Get(), PhysX::PxTransform(PhysX::PxVec3(0.0f, 3.0f, 0.0f)));
			System->SetColliderForRigidBody(Result);

			return Result;
		}
	}
}