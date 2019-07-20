#pragma once
#include "Common.h"

class DemoGame : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> mRenderSystem;
	//std::shared_ptr<Systems::AudioSystem> mAudioSystem;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	Core::Input Input;

	Managers::AssetManager AssetLoader;
	Managers::CameraManager SceneCameraManager;

	Graphics::PBR PBR;
	Graphics::BlinnPhong BlinnPhong;
	Components::CameraComponent Camera;

	ECS::Scene Scene;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

	void SetupEntities()
	{

	}
	void InitRenderer()
	{
		mRenderSystem = Scene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		Systems::PhysXSystemDesc sceneDesc;

		sceneDesc.mGravity = Math::Vector3(0.0f, -9.81f, 0.0f);

		mPhysXSystem = Scene.Systems.Add<Systems::PhysXSystem>(&Scene, sceneDesc);

		Scene.Systems.Configure();
		mRenderSystem->AddRenderingPipeline(&PBR);
		mRenderSystem->Bake(Scene.Entities);

		mRenderSystem->SetActiveRenderingPipeline(PBR.GetID());
	}

	void Load()
	{
		Assets::DefaultTextures::Initalize(&AssetLoader);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		InitRenderer();

		SetupEntities();
		mPhysXSystem->Bake(Scene.Entities);
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
		mRenderSystem->Update(Scene.Entities, Scene.Events, dt);

		{
			using namespace Graphics;
			ImGui::Begin("Demo Game");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
			ImGui::ColorEdit3("Camera ClearColor", (float*)& Camera.RTClearColor);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};