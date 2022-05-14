#pragma once
#include "Common.h"

class Sample1 : public Client
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
	std::shared_ptr<Systems::LightingSystem> mLightingSystem;

	Assets::Mesh* NanosuitAsset;
	Assets::Mesh* CyborgAsset;
	Assets::Mesh* BobAsset;
	Assets::Mesh* VampireAsset;

	Assets::Material CubeMaterial;
	Assets::Material* NanosuitMaterial;
	Assets::Material* CyborgMaterial;
	Assets::Material* BobMaterial;
	Assets::Material* VampireMaterial;

	Animation::Animator BobAnimator;
	Animation::Animator VampireAnimator;

	Assets::Animations* BobAnimation;
	Assets::Animations* VampireAnimation;

	Graphics::Camera Camera;

	Rendering::Skybox Skybox;

	Rendering::DiffuseOnly DiffuseRP;
	Rendering::WireFrame WireFrameRP;
	Rendering::BlinnPhong BlinnPhongRP;
	Rendering::BlinnPhong DefferedBlinnPhong;

	Rendering::ForwardRenderingPipeline BlinnPhongPipeline;
	Rendering::ForwardRenderingPipeline DiffuseRPPipeline;
	Rendering::ForwardRenderingPipeline WireFrameRPPipeline;
	Rendering::DefferedRenderingPipeline DefferedPipeline;

	ECS::Entity ECube;
	ECS::Entity ECyborg;
	ECS::Entity ENanosuit;
	ECS::Entity EBob;
	ECS::Entity EVampire;

	ECS::Scene Scene;
	ECS::Entity EController;
	ECS::Entity ELights;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

public:
	Sample1()
		: Camera(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 1.0f, 0.0f),  Graphics::YAW, Graphics::PITCH, Graphics::SPEED, Graphics::SENSITIVTY, Graphics::ZOOM)
	{
	}
	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;
		Assets::Animations* Placeholder;

		//Load Nanosuit Model
		std::tie(NanosuitAsset, NanosuitMaterial, Placeholder) = mAssetManager->Import("@CommonAssets@/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);
		
		//Load Cyborg Model
		std::tie(CyborgAsset, CyborgMaterial, Placeholder) = mAssetManager->Import("@CommonAssets@/Models/CrytekCyborg/cyborg.obj", ModelDesc);
		
		//Load Bob Model
		std::tie(BobAsset, BobMaterial, BobAnimation) = mAssetManager->Import("@CommonAssets@/Models/Bob/boblampclean.md5mesh", ModelDesc);

		//Load Bob Model
		std::tie(VampireAsset, VampireMaterial, VampireAnimation) = mAssetManager->Import("@CommonAssets@/Models/vampire/vampire_a_lusth.fbx", ModelDesc);

		BobAnimator.Initialize(&BobAnimation->mClips.at(0));
		VampireAnimator.Initialize(&VampireAnimation->mClips.at(0));

		//Load some textures manually
		Importers::ImageLoadingDesc desc;
	//	desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/crate_diffuse.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		CubeSet.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/crate_specular.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Specular) });
		CubeSet.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/crate_normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });

		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		Renderer->CreateMaterialForAllPipelines(&CubeMaterial);
		Renderer->CreateMaterialForAllPipelines(NanosuitMaterial);
		Renderer->CreateMaterialForAllPipelines(CyborgMaterial);
		Renderer->CreateMaterialForAllPipelines(BobMaterial);
		Renderer->CreateMaterialForAllPipelines(VampireMaterial);

		CubeSet.mData.clear();

		ECube.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), &CubeMaterial);
		ENanosuit.AddComponent<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
		ECyborg.AddComponent<Components::MeshComponent>(CyborgAsset, CyborgMaterial);

		EBob.AddComponent<Components::MeshComponent>(BobAsset, BobMaterial);
		EBob.AddComponent<Components::AnimatorComponent>(&BobAnimator);

		EVampire.AddComponent<Components::MeshComponent>(VampireAsset, VampireMaterial);
		EVampire.AddComponent<Components::AnimatorComponent>(&VampireAnimator);

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
		//SkyboxDesc.mFormat = TEX_FORMAT_RGBA8_UNORM;
		auto test = mAssetManager->LoadTextureCubeFromFile(SkyBoxTexturePaths, SkyboxDesc);
		Skybox.Initialize(mCameraSystem->GetCameraCB(), test);
	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform TNansosuit, TCyborg, TBob, TVampire, TCube;
		TNansosuit.SetPosition(Math::Vector3(0.0f, -1.75f, 0.0f));
		TNansosuit.SetScale(Math::Vector3(0.3f, 0.3f, 0.3f));

		TCyborg.SetPosition(Math::Vector3(4.0f, -1.75f, 0.0f));

		TBob.SetPosition(Math::Vector3(-4.0f, -1.75f, 0.0f));
		TBob.SetScale(Math::Vector3(0.07f, 0.07f, 0.07f));

		TVampire.SetPosition(Math::Vector3(-4.0f, -1.75f, 4.0f));
		TVampire.SetScale(Math::Vector3(0.02f, 0.02f, 0.02f));

		TCube.SetPosition(Math::Vector3(2.0f, -1.75f, 2.0f));
		TCube.SetScale(Math::Vector3(2.f, 2.f, 2.f));

		ECube = Scene.CreateEntity("Cube", TCube);
		ENanosuit = Scene.CreateEntity("Nanosuit", TNansosuit);
		ECyborg = Scene.CreateEntity("Cyborg", TCyborg);
		EBob = Scene.CreateEntity("Bob", TBob);
		EVampire = Scene.CreateEntity("Vampire" , TVampire);
		ELights = Scene.CreateEntity("Lights");

		//ENanosuit.Assign<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
		ELights.AddComponent<Components::DirLightComponent>();
		ELights.AddComponent<Components::PointLightComponent>();


		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Math::Vector3(0.7f, 0.2f, 2.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.f);
	}

	void InitRenderer()
	{
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();

		BlinnPhongPipeline.Initialize(&BlinnPhongRP, &Camera);
		DiffuseRPPipeline.Initialize(&DiffuseRP, &Camera);
		WireFrameRPPipeline.Initialize(&WireFrameRP, &Camera);
		DefferedBlinnPhong.Initialize({ true });
		Rendering::DefferedRenderingPipelineInitInfo initInfo;
		initInfo.shadingModel = &DefferedBlinnPhong;
		initInfo.camera = &Camera;
		DefferedPipeline.Initialize(initInfo);
		//Scene.Systems.Configure();
		//TestPBR.test = true;
		//Renderer->AddRenderingPipeline(&TestPBR);
		Renderer->AddRenderingPipeline(&BlinnPhongPipeline);
		Renderer->AddRenderingPipeline(&DiffuseRPPipeline);
		Renderer->AddRenderingPipeline(&WireFrameRPPipeline);
		Renderer->AddRenderingPipeline(&DefferedPipeline);


		Renderer->Bake(_Width_, _Height_);
	}

	void Load()
	{
		mAssetManager->Initialize();

		EController = Scene.CreateEntity();
		EController.AddComponent<Components::SpotLightComponent>();
		EController.AddComponent<Components::CameraComponent>(&Camera);

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
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
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
		EController.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Camera.GetPosition());

		Renderer->GetActivePipeline()->UpdatePSO();
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };
		BobAnimator.UpdateAnimation(dt);
		VampireAnimator.UpdateAnimation(dt);

		EController.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());

		Scene.Update(dt);
		{
			using namespace Graphics;
			ImGui::Begin("Sample1: Basic Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Text("Active Rendering Pipeline:");
			static int e = 0;
			ImGui::RadioButton("DiffuseOnly", &e, 0);
			ImGui::RadioButton("BlinnPhong", &e, 1);
			//ImGui::RadioButton("BlinnPhongWithNormalMap", &e, 2);
			ImGui::RadioButton("WireFrame", &e, 3);
			ImGui::RadioButton("Deffered Blinn Phong", &e, 4);

			//Change Rendering Pipeline

			if (e == 0)
				Renderer->SetActiveRenderingPipeline(DiffuseRPPipeline.GetID());
			else if (e == 1)
				Renderer->SetActiveRenderingPipeline(BlinnPhongPipeline.GetID());
			//else if (e == 2)

			else if (e == 3)
				Renderer->SetActiveRenderingPipeline(WireFrameRPPipeline.GetID());
			else if (e == 4)
				Renderer->SetActiveRenderingPipeline(DefferedPipeline.GetID());


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

			//ImGui::Checkbox("Render Skybox", &Camera.RenderSkybox);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Engine::GetInstance()->EndClient();
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