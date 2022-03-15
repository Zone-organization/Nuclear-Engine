#pragma once
#include "Common.h"

class Sample3 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	Core::Input Input;

	Managers::CameraManager SceneCameraManager;

	Assets::Material PlaneMaterial;
	Assets::Material CubeMaterial;
	Components::CameraComponent Camera;

	Graphics::DiffuseOnly DiffuseRP;

	ECS::Scene Scene;
	ECS::Entity EPlane;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

	void SetupEntities()
	{
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse) });

		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		Renderer->CreateMaterialForAllPipelines(&CubeMaterial);

		Assets::TextureSet PlaneSet;
		PlaneSet.mData.push_back({ 0, Assets::DefaultTextures::DefaultGreyTex });

		PlaneMaterial.mPixelShaderTextures.push_back(PlaneSet);

		Renderer->CreateMaterialForAllPipelines(&PlaneMaterial);
		Scene.Factory.InitializeDefaultPhysxMaterials();

		//Create Entities
		EPlane = Scene.CreateEntity();

		static Assets::Mesh gPlane;
		Assets::Mesh::CreatePlane(&gPlane, Assets::MeshVertexDesc(), 100.0f, 100.0f);
		//Assign Components
		EPlane.Assign<Components::MeshComponent>(&gPlane, &PlaneMaterial);
		EPlane.Assign<Components::ColliderComponent>(Scene.Factory.GetDefaultPlaneMaterial(), PhysX::PlaneGeometry(ECS::Transform()));
		mPhysXSystem->AddActor(EPlane);
	}
	void InitRenderer()
	{
		Renderer = Scene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		Systems::PhysXSystemDesc sceneDesc;
		
		sceneDesc.mGravity = Math::Vector3(0.0f, -9.81f, 0.0f);

		mPhysXSystem = Scene.Systems.Add<Systems::PhysXSystem>(&Scene, sceneDesc);

		Scene.Systems.Configure();
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->Bake(Scene.Entities);

		Renderer->SetActiveRenderingPipeline(DiffuseRP.GetID());
	}

	void Load()
	{
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		InitRenderer();

		SetupEntities();
		Components::CameraBakingOptions Desc;
		Desc.RTWidth = _Width_;
		Desc.RTHeight = _Height_;
		Camera.Bake(Desc);
		Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
	}


	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Camera.ResizeRenderTarget(width, height);
	}

	void OnMouseMovement(int xpos_a, int ypos_a) override
	{
		if (!isMouseDisabled)
		{
			float xpos = static_cast<float>(xpos_a);
			float ypos = static_cast<float>(ypos_a);

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			Camera.ProcessEye(xoffset, yoffset);
		}
	}

	bool RenderSponza = false;

	void Update(float deltatime) override
	{
		//Movement
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::CAMERA_MOVEMENT_RIGHT, deltatime);

		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_LEFT_SHIFT) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.MovementSpeed = 10;
		else
			Camera.MovementSpeed = 4.5;

		//Change Mouse Mode
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera.Update();
		SceneCameraManager.UpdateBuffer();
	}
	void Render(float dt) override
	{
		mPhysXSystem->BeginSimulation(dt);

		// Clear the back buffer 
		const float ClearColor[] = { 0.3f,  0.3f,  0.3f, 1.0f };

		mPhysXSystem->Update(Scene.Entities, Scene.Events, dt);		
		Renderer->Update(Scene.Entities, Scene.Events, dt);


		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_SPACE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			auto ECube = Scene.Factory.CreateBox(&CubeMaterial, ECS::Transform(Camera.GetPosition(), Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f)));
			Math::Matrix4 TCube(1.0f);
			TCube = Math::translate(TCube, Camera.GetPosition());
			ECube.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TCube);
		}

		{
			using namespace Graphics;
			ImGui::Begin("Sample3: PhysX Integration");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
			ImGui::ColorEdit3("Camera ClearColor", (float*)& Camera.RTClearColor);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Core::Engine::EndGame();
			}
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};