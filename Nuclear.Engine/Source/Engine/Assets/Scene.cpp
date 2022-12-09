#include <Engine\Assets\Scene.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <Engine\Components\Components.h>
#include <Engine/Serialization/ComponentsArchive.h>
#include <Engine\Systems\PhysXSystem.h>
#include <Engine/Assets/DefaultMeshes.h>
#include "..\PhysX\PhysXTypes.h"
#include <Engine/Assets/Material.h>
#include <Engine\Managers\SceneManager.h>
#include <Engine.h>
#include <Client.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Assets
	{
		Scene::Scene()
		{
		}

		Scene::~Scene()
		{
		//	Entities.reset();
		}

		ECS::Entity Scene::CreateEntity()
		{
			ECS::Entity result(GetRegistry(), GetRegistry().create());
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>();
			einfo.mOwnerEntity = result;

			return result;
		}
		ECS::Entity Scene::CreateEntity(const std::string& name, const ECS::Transform& transform)
		{
			ECS::Entity result(GetRegistry(), GetRegistry().create());
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>(transform, name);
			einfo.mOwnerEntity = result;

			return result;
		}

		ECS::Entity Scene::CreateBox(Assets::Material* material, const ECS::Transform& transform, bool addcollider)
		{
			ECS::Entity Result = CreateEntity("Box", transform);

			Result.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), material);

			if (addcollider)
			{
				auto System = GetSystemManager().GetSystem<Systems::PhysXSystem>();
				if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
				{
					PhysX::BoxGeometry box(Math::Vector3(0.5f, 0.5f, 0.5f), transform);
					Components::ColliderDesc desc;
					desc.mGeo = &box;
					desc.mShape = Components::ColliderShape::Box;
					Result.AddComponent<Components::ColliderComponent>(desc);
					Result.AddComponent<Components::RigidBodyComponent>(transform);
					System->AddActor(Result);
					System->SetColliderForRigidBody(Result);
					Result.GetComponent<Components::ColliderComponent>().SetValid(true);
				}
			}

			return Result;
		}

		ECS::Entity Scene::CreateSphere(Assets::Material* material, const ECS::Transform& transform, bool addcollider)
		{
			ECS::Entity Result = CreateEntity("Sphere", transform);
			Result.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), material);

			if (addcollider)
			{
				auto System = GetSystemManager().GetSystem<Systems::PhysXSystem>();
				if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
				{
					PhysX::SphereGeometry sphere(1.0f, transform);
					Components::ColliderDesc desc;
					desc.mGeo = &sphere;
					desc.mShape = Components::ColliderShape::Sphere;
					Result.AddComponent<Components::ColliderComponent>(desc);
					Result.AddComponent<Components::RigidBodyComponent>(transform);
					System->AddActor(Result);
					System->SetColliderForRigidBody(Result);
					Result.GetComponent<Components::ColliderComponent>().SetValid(true);
				}
			}

			return Result;
		}

		ECS::Entity Scene::CreatePlane(Assets::Material* material, const ECS::Transform& transform, bool addcollider)
		{
			ECS::Entity Result = CreateEntity("Plane", transform);
			Result.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetPlaneAsset(), material);

			if (addcollider)
			{
				auto System = GetSystemManager().GetSystem<Systems::PhysXSystem>();
				if (System.get() != nullptr && System->GetPhysXScene() != nullptr)
				{
					PhysX::PlaneGeometry plane(transform);
					Components::ColliderDesc desc;
					desc.mGeo = &plane;
					desc.mShape = Components::ColliderShape::Plane;
					Result.AddComponent<Components::ColliderComponent>(desc);
					System->AddActor(Result);
				}
			}

			return Result;
		}

		ECS::SystemManager& Scene::GetSystemManager()
		{

			if (ParentSceneMgr->GetActiveScene() != this)
			{
				NUCLEAR_ERROR("[Scene] [GetSystemManager] Active Scene mismatch!");
			}
			return ParentSceneMgr->GetSystemManager();
		}

		entt::registry& Scene::GetRegistry()
		{
			if (ParentSceneMgr->GetActiveScene() != this)
			{
				NUCLEAR_ERROR("[Scene] [GetRegistry] Active Scene mismatch!");
			}
			return ParentSceneMgr->GetRegistry();
		}

	/*	void Scene::Save()
		{
			std::stringstream storage;

			entt::registry destination;

			{
				// output finishes flushing its contents when it goes out of scope
				cereal::JSONOutputArchive output{ storage };
				entt::snapshot{ GetRegistry()}.entities(output).component<
					Components::EntityInfoComponent,
					Components::MeshComponent

				//	Components::DirLightComponent,
				//	Components::PointLightComponent,
				//	Components::SpotLightComponent
				>(output);
			}

			cereal::JSONInputArchive input{ storage };
			entt::snapshot_loader{ destination }.entities(input).component<Components::EntityInfoComponent, Components::MeshComponent
			>(input);
		}*/
	}
}