#pragma once
#include "Common.h"

class Sample4 : public Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
	std::shared_ptr<Systems::LightingSystem> mLightingSystem;

	Assets::Mesh* SponzaAsset;
	Assets::Material* SponzaMaterial;

	Graphics::Camera Camera;

	Rendering::Skybox Skybox;

	Rendering::PBR PBR;
	Rendering::DiffuseOnly DiffuseRP;
	Rendering::WireFrame WireFrameRP;
	Rendering::BlinnPhong BlinnPhongRP;

	Rendering::ForwardRenderingPipeline PBRPipeline;
	Rendering::ForwardRenderingPipeline BlinnPhongPipeline;
	Rendering::ForwardRenderingPipeline DiffuseRPPipeline;
	Rendering::ForwardRenderingPipeline WireFrameRPPipeline;
	ECS::Scene Scene;

	ECS::Entity ESponza;

	ECS::Entity ECamera;
	ECS::Entity ELights;

	// positions of the point lights
	Math::Vector3 pointLightPositions[9] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f),
		Math::Vector3(4.0f,  3.0f, -2.0f),
		Math::Vector3(6.2f, 2.0f, 0.0f),
		Math::Vector3(6.2f, -2.0f, 0.0f),
		Math::Vector3(-6.2f, 2.0f, 0.0f),
		Math::Vector3(-6.2f, -2.0f, 0.0f)
	};
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

public:
	Sample4()
		: Camera(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, Graphics::SPEED, Graphics::SENSITIVTY, Graphics::ZOOM),
		BlinnPhongPipeline("BlinnPhong"),
		DiffuseRPPipeline("DiffuseRP"),
		WireFrameRPPipeline("WireFrameRP"),
		PBRPipeline("PBR")
	{
	}
	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;
		Assets::Animations* Placeholder;

		//Load Sponza Model
		std::tie(SponzaAsset, SponzaMaterial, Placeholder) = mAssetManager->Import("@CommonAssets@/Models/CrytekSponza/sponza.fbx", ModelDesc);

		Renderer->CreateMaterialForAllPipelines(SponzaMaterial);

		ESponza.AddComponent<Components::MeshComponent>(SponzaAsset, SponzaMaterial);

		//Create The skybox
		std::array<Core::Path, 6> SkyBoxTexturePaths
		{
			Core::Path("@CommonAssets@/Skybox/right.jpg"),
			Core::Path("@CommonAssets@/Skybox/left.jpg"),
			Core::Path("@CommonAssets@/Skybox/top.jpg"),
			Core::Path("@CommonAssets@/Skybox/bottom.jpg"),
			Core::Path("@CommonAssets@/Skybox/front.jpg"),
			Core::Path("@CommonAssets@/Skybox/back.jpg")
		};

		Importers::ImageLoadingDesc SkyboxDesc;
	//	SkyboxDesc.mFormat = TEX_FORMAT_RGBA8_UNORM;
		auto test = mAssetManager->LoadTextureCubeFromFile(SkyBoxTexturePaths, SkyboxDesc);
		Skybox.Initialize(mCameraSystem->GetCameraCB(), test);
	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform TSponza;
		TSponza.SetScale(Math::Vector3(0.05f, 0.05f, 0.05f));
		ESponza = Scene.CreateEntity("Sponza", TSponza);
		ELights = Scene.CreateEntity("Lights");

		ELights.AddComponent<Components::DirLightComponent>();
		ELights.AddComponent<Components::PointLightComponent>();


		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));
		ELights.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(pointLightPositions[0]);
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.f);

		for (int i = 1; i < 9; i++)
		{
			auto Light = Scene.CreateEntity("Light" + std::to_string(i));
			Light.AddComponent<Components::PointLightComponent>();
			Light.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(pointLightPositions[i]);
			Light.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
			Light.GetComponent<Components::PointLightComponent>()->SetIntensity(2.f);
		}

	}

	void InitRenderer()
	{
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();

		BlinnPhongPipeline.Initialize(&BlinnPhongRP, &Camera);
		DiffuseRPPipeline.Initialize(&DiffuseRP, &Camera);
		WireFrameRPPipeline.Initialize(&WireFrameRP, &Camera);
		PBRPipeline.Initialize(&PBR, &Camera);

		Renderer->AddRenderingPipeline(&PBRPipeline);
		Renderer->AddRenderingPipeline(&BlinnPhongPipeline);
		Renderer->AddRenderingPipeline(&DiffuseRPPipeline);
		Renderer->AddRenderingPipeline(&WireFrameRPPipeline);

		Renderer->Bake(_Width_, _Height_);
	}

	void Load()
	{
		mAssetManager->Initialize();

		ECamera = Scene.CreateEntity();
		ECamera.AddComponent<Components::SpotLightComponent>();
		ECamera.AddComponent<Components::CameraComponent>(&Camera);

		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));

		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		mLightingSystem = Scene.GetSystemManager().Add<Systems::LightingSystem>();
		SetupEntities();
		mLightingSystem->Bake();

		InitRenderer();

		SetupAssets();

		Renderer->GetBackground().SetSkybox(&Skybox);
		Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
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


	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 400.0f));
		Renderer->ResizeRenderTargets(width, height);
	}
	void Update(float deltatime) override
	{
		//Movement
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_RIGHT, deltatime);

		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_LEFT_SHIFT) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.MovementSpeed = 10;
		else
			Camera.MovementSpeed = 4.5;

		//Change Mouse Mode
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera.UpdateBuffer();
		mCameraSystem->Update(deltatime);
		Renderer->GetActivePipeline()->UpdatePSO();
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		ECamera.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera.GetPosition());
		ECamera.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());

		Scene.Update(dt);
		{
			using namespace Graphics;
			ImGui::Begin("Sample3: Sponza Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Text("Active Rendering Pipeline:");
			static int e = 0;
			ImGui::RadioButton("DiffuseOnly", &e, 0);
			ImGui::RadioButton("BlinnPhong", &e, 1);
			//ImGui::RadioButton("BlinnPhongWithNormalMap", &e, 2);
			ImGui::RadioButton("WireFrame", &e, 3);
			ImGui::RadioButton("PBR", &e, 4);

			//Change Rendering Pipeline

			if (e == 0)
				Renderer->SetActiveRenderingPipeline(DiffuseRPPipeline.GetID());
			else if (e == 1)
				Renderer->SetActiveRenderingPipeline(BlinnPhongPipeline.GetID());
			//else if (e == 2)
			//	Renderer->SetActiveRenderingPipeline(BlinnPhongNormalPipeline.GetID());
			else if (e == 3)
				Renderer->SetActiveRenderingPipeline(WireFrameRPPipeline.GetID());
			else if (e == 4)
				Renderer->SetActiveRenderingPipeline(PBRPipeline.GetID());

			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);


			if (ImGui::TreeNode("Pipeline Effects"))
			{
				for (auto& it : Renderer->GetActivePipeline()->mPairedEffects)
				{
					bool value = it.second.GetValue();
					ImGui::Checkbox(it.second.GetName().c_str(), &value);
					if (value != it.second.GetValue())
					{
						Renderer->GetActivePipeline()->SetEffect(it.second.GetID(), value);
					}
				}
				ImGui::TreePop();
			}

			//ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);

		//	ImGui::Checkbox("Render Skybox", &Camera.RenderSkybox);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Engine::GetInstance()->EndGame();
			}

			ImGui::End();
			EntityExplorer(&Scene);
		}
	}

	void Shutdown() override
	{
		mAssetManager->FlushContainers();
	}
};