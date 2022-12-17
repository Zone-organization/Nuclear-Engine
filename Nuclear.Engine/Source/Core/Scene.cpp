#include <Core\Scene.h>
#include <sstream>
#include <Components\Components.h>
#include <Systems\PhysXSystem.h>
#include <Assets/DefaultMeshes.h>
#include "..\PhysX\PhysXTypes.h"
#include <Assets/Material.h>
#include <Core/Engine.h>
#include <Utilities/Logger.h>

#include "Serialization/MathSerialization.h"
#include "Serialization/SceneArchive.h"

#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/string.h>
#include <Assets/AssetManager.h>


namespace Nuclear 
{
	namespace Core
	{
		Scene& Scene::GetInstance()
		{
			static Scene instance;

			return instance;
		}
		ECS::Entity Scene::CreateEntity()
		{
			ECS::Entity result(GetRegistry(), GetRegistry().create());
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>();

			return result;
		}
		ECS::Entity Scene::CreateEntity(const std::string& name, const ECS::Transform& transform)
		{
			ECS::Entity result(GetRegistry(), GetRegistry().create());
			auto& einfo = result.AddComponent<Components::EntityInfoComponent>(transform, name);

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

		void Scene::SetMainCamera(const ECS::Entity& entity)
		{
			pMainCamera = entity.TryToGetComponent<Components::CameraComponent>();
		}

		void Scene::SetMainCamera(Components::CameraComponent* camera)
		{
			pMainCamera = camera;
		}

		Components::CameraComponent* Scene::GetMainCamera()
		{
			return pMainCamera;
		}

		ECS::SystemManager& Scene::GetSystemManager()
		{
			return mSystems;
		}

		entt::registry& Scene::GetRegistry()
		{
			return mRegistry;
		}

		void Scene::Update(ECS::TimeDelta dt)
		{
			return mSystems.Update_All(dt);
		}

		// some helper types
		using Buffer = std::vector<Uint8>;
		using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
		using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

		bool Scene::SaveScene(Assets::SavedScene* scene)
		{
			scene->mBinaryBuffer.clear();

			bitsery::Serializer<OutputAdapter> ser{ scene->mBinaryBuffer };

			Serialization::SceneOutputArchive<bitsery::Serializer<OutputAdapter>> output(&ser);
			entt::snapshot{ mRegistry }.entities(output).component < Components::EntityInfoComponent, Components::LightComponent>(output);

			ser.object(Assets::AssetManager::GetInstance().mLibrary);


			return true;
		}
		bool Scene::LoadScene(Assets::SavedScene* scene)
		{
			mRegistry.clear();


			bitsery::Deserializer<InputAdapter> deser{ scene->mBinaryBuffer.begin(), scene->mBinaryBuffer.size()};
			Serialization::SceneInputArchive<bitsery::Deserializer<InputAdapter>> input(&deser);


			entt::snapshot_loader{ mRegistry }.entities(input).component<Components::EntityInfoComponent, Components::LightComponent>(input);

			return true;
		}

		Scene::Scene()
		{	
			pMainCamera = nullptr;
		}
	}
}