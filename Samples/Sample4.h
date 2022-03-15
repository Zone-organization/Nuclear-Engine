#pragma once
#include "Common.h"

class Sample4 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;

	Managers::CameraManager SceneCameraManager;

	Assets::Mesh* NanosuitAsset;
	Assets::Mesh* CyborgAsset;

	Assets::Material CubeMaterial;
	Assets::Material* NanosuitMaterial;
	Assets::Material* CyborgMaterial;

	Components::CameraComponent Camera;

	Graphics::Skybox Skybox;

	Graphics::DiffuseOnly DiffuseRP;
	Graphics::WireFrame WireFrameRP;
	Graphics::BlinnPhong BlinnPhongRP;
	Graphics::BlinnPhong BlinnPhongWithNormalMapRP = Graphics::BlinnPhong(true);

	//ECS
	ECS::Scene ModelsScene;
	ECS::Entity ECube;
	ECS::Entity ECyborg;
	ECS::Entity ENanosuit;

	ECS::Entity ECamera;
	ECS::Entity ELights;

	// positions all containers
	Math::Vector3 cubePositions[10] =
	{
		Math::Vector3(0.0f,  0.0f,  0.0f),
		Math::Vector3(2.0f,  5.0f, -15.0f),
		Math::Vector3(-1.5f, -2.2f, -2.5f),
		Math::Vector3(-3.8f, -2.0f, -12.3f),
		Math::Vector3(2.4f, -0.4f, -3.5f),
		Math::Vector3(-1.7f,  3.0f, -7.5f),
		Math::Vector3(1.3f, -2.0f, -2.5f),
		Math::Vector3(1.5f,  2.0f, -2.5f),
		Math::Vector3(1.5f,  0.2f, -1.5f),
		Math::Vector3(-1.3f,  1.0f, -1.5f)
	};

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

	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;

		//Load Nanosuit Model
		std::tie(NanosuitAsset, NanosuitMaterial) = mAssetManager->Import("@CommonAssets@/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);
		//Load Cyborg Model
		std::tie(CyborgAsset, CyborgMaterial) = mAssetManager->Import("@CommonAssets@/Models/CrytekCyborg/cyborg.obj", ModelDesc);

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse) });
		CubeSet.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/crate_specular.png", Assets::TextureUsageType::Specular) });
		CubeSet.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/crate_normal.png", Assets::TextureUsageType::Normal) });

		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		Renderer->CreateMaterialForAllPipelines(&CubeMaterial);
		Renderer->CreateMaterialForAllPipelines(NanosuitMaterial);
		Renderer->CreateMaterialForAllPipelines(CyborgMaterial);

		CubeSet.mData.clear();

		ECube.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), &CubeMaterial);
		ENanosuit.Assign<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
		ECyborg.Assign<Components::MeshComponent>(CyborgAsset, CyborgMaterial);

		//CubeMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//CubeMaterial.SetMaterialVariable("Shininess", 64.0f);

		//NanosuitMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//NanosuitMaterial.SetMaterialVariable("Shininess", 64.0f);

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

		Importers::TextureLoadingDesc SkyboxDesc;
		SkyboxDesc.mFormat = TEX_FORMAT_RGBA8_UNORM;
		Skybox.Initialize(SceneCameraManager.GetCameraCB(), mAssetManager->LoadTextureCubeFromFile(SkyBoxTexturePaths, SkyboxDesc));
	}
	void SetupEntities()
	{
		//Create Entities
		ECube = ModelsScene.CreateEntity();
		ENanosuit = ModelsScene.CreateEntity();
		ECyborg = ModelsScene.CreateEntity();
		ECamera = ModelsScene.CreateEntity();
		ELights = ModelsScene.CreateEntity();

		//ENanosuit.Assign<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
		ELights.Assign<Components::DirLightComponent>();
		ELights.Assign<Components::PointLightComponent>();
		ECamera.Assign<Components::SpotLightComponent>();

		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::PointLightComponent>()->SetPosition(pointLightPositions[0]);
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
	}

	void InitRenderer()
	{
		Renderer = ModelsScene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		ModelsScene.Systems.Configure();

		Renderer->AddRenderingPipeline(&BlinnPhongRP);
		Renderer->AddRenderingPipeline(&BlinnPhongWithNormalMapRP);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);
		Renderer->Bake(ModelsScene.Entities);

	}

	void Load()
	{
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		SetupEntities();

		InitRenderer();

		SetupAssets();

		//Setup positions
		Math::Matrix4 TNanosuit(1.0f);
		TNanosuit = Math::translate(TNanosuit, Math::Vector3(0.0f, -1.75f, 0.0f));
		TNanosuit = Math::scale(TNanosuit, Math::Vector3(0.3f, 0.3f, 0.3f));
		ENanosuit.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TNanosuit);

		Math::Matrix4 TCyborg(1.0f);
		TCyborg = Math::translate(TCyborg, Math::Vector3(4.0f, -1.75f, 0.0f));
		ECyborg.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TCyborg);

		Math::Matrix4 TCube(1.0f);
		TCube = Math::translate(TCube, Math::Vector3(2.0f, -1.75f, 2.0f));
		ECube.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TCube);

		Components::CameraBakingOptions Desc;
		Desc.RTWidth = _Width_;
		Desc.RTHeight = _Height_;
		Camera.Bake(Desc);
		Camera.mSkybox = &Skybox;
		Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
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
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Camera.ResizeRenderTarget(width, height);
	}
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
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		ECamera.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera.GetPosition());
		ECamera.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());

		Renderer->Update(ModelsScene.Entities, ModelsScene.Events, dt);

		{
			using namespace Graphics;
			ImGui::Begin("Sample4: Scene Loading");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Text("Active Rendering Pipeline:");
			static int e = 0;
			ImGui::RadioButton("DiffuseOnly", &e, 0);
			ImGui::RadioButton("BlinnPhong", &e, 1);
			ImGui::RadioButton("BlinnPhongWithNormalMap", &e, 2);
			ImGui::RadioButton("WireFrame", &e, 3);

			//Change Rendering Pipeline

			if (e == 0)
				Renderer->SetActiveRenderingPipeline(DiffuseRP.GetID());
			else if (e == 1)
				Renderer->SetActiveRenderingPipeline(BlinnPhongRP.GetID());
			else if (e == 2)
				Renderer->SetActiveRenderingPipeline(BlinnPhongWithNormalMapRP.GetID());
			else if (e == 3)
				Renderer->SetActiveRenderingPipeline(WireFrameRP.GetID());


			ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);

			ImGui::Checkbox("Render Skybox", &Camera.RenderSkybox);

			ImGui::ColorEdit3("Camera ClearColor", (float*)& Camera.RTClearColor);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};