#pragma once
#include "Common.h"


//Current TODO:
//Shadow test
class Playground : public Client
{
	Managers::AssetManager AssetLoader;

	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
	std::shared_ptr<Systems::LightingSystem> mLightingSystem;
	std::shared_ptr<Systems::DebugSystem> mDebugSystem;

	Assets::Material SphereMaterial;
	Assets::Material PlaneMaterial;

	Assets::Font* ArialFont;

	Graphics::Camera Camera;

	Rendering::BlinnPhong BlinnphongRP;
	Rendering::ForwardRenderingPipeline BlinnPhongPipeline;
	Rendering::PBR PBR;
	Rendering::ForwardRenderingPipeline PBRPipeline;

	Rendering::ShadowPass ShadowPass;

	Assets::Scene Scene;

	ECS::Entity EController;
	ECS::Entity ELights;


	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Playground()
		: Camera(Math::Vector3(0.0f, 5.0f, 30.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, 10.f, Graphics::SENSITIVTY, Graphics::ZOOM)
	{
		AssetLoader.Initialize();

		mAssetManager = &AssetLoader;
	}
	void SetupAssets()
	{
		//Load some textures manually
		Importers::ImageLoadingDesc desc;
		//desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRRustedIron.mData.push_back({ 1, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRRustedIron.mData.push_back({ 2, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRRustedIron.mData.push_back({ 3, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRRustedIron.mData.push_back({ 4, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		SphereMaterial.mPixelShaderTextures.push_back(PBRRustedIron);
		SphereMaterial.SetName("RustedIron Material");

		Assets::TextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/PBR/plastic/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRPlastic.mData.push_back({ 1, mAssetManager->Import("Assets/Common/Textures/PBR/plastic/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRPlastic.mData.push_back({ 2, mAssetManager->Import("Assets/Common/Textures/PBR/plastic/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRPlastic.mData.push_back({ 3, mAssetManager->Import("Assets/Common/Textures/PBR/plastic/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRPlastic.mData.push_back({ 4, mAssetManager->Import("Assets/Common/Textures/PBR/plastic/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		PlaneMaterial.mPixelShaderTextures.push_back(PBRPlastic);
		PlaneMaterial.SetName("Plastic Material");

		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
		Renderer->CreateMaterialForAllPipelines(&PlaneMaterial);

		Importers::FontLoadingDesc fdesc;
		ArialFont = mAssetManager->Import("Assets/Common/Fonts/arial.ttf", fdesc);
	}
	void SetupEntities()
	{
		//Create Entities
		ELights = Scene.CreateEntity("Lights");
		EController = Scene.CreateEntity("Controller");

		//Assign Components
		ELights.AddComponent<Components::DirLightComponent>().mCastShadows = true;
	//	ELights.AddComponent<Components::PointLightComponent>();
		//EController.AddComponent<Components::SpotLightComponent>();
		EController.AddComponent<Components::CameraComponent>(&Camera);

		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));

		//ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(1.f));

		ELights.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Math::Vector3(-2.0f, 4.0f, -1.0f));
	//	ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		//ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);

	}
	void InitRenderer()
	{

		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();
		Rendering::ShadingModelInitInfo info;
		info.ShadowingEnabled = true;
		BlinnphongRP.Initialize(info);
		
		BlinnPhongPipeline.Initialize(&BlinnphongRP, &Camera);

		PBRPipeline.Initialize(&PBR, &Camera);

		//FwdPipeline.Initialize(&BlinnphongRP, &Camera);

		Renderer->AddRenderingPipeline(&BlinnPhongPipeline);
		Renderer->AddRenderingPipeline(&PBRPipeline);

		Renderer->Bake(_Width_, _Height_);
	}

	void Load()
	{
		mSceneManager->CreateScene(&Scene, true);

		mDebugSystem = Scene.GetSystemManager().Add<Systems::DebugSystem>();

		mAssetManager->Initialize();
		Systems::LightingSystemDesc desc;
		desc.ShadowPass = &ShadowPass;
		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		mLightingSystem = Scene.GetSystemManager().Add<Systems::LightingSystem>(desc);
		SetupEntities();
		mLightingSystem->Bake();

		InitRenderer();

		SetupAssets();


		// cubes
		{
			ECS::Transform TSphere;
			TSphere.SetPosition(Math::Vector3(0.0f, 1.5f, 0.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			Scene.CreateBox(&SphereMaterial, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(2.0f, 0.0f, 1.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			Scene.CreateBox(&SphereMaterial, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(-1.0f, 0.0f, 2.0));
			TSphere.SetRotation(glm::normalize(glm::vec3(1.0, 0.0, 1.0)), glm::radians(60.0f));
	//		TSphere.SetScale(Math::Vector3(0.25));
			Scene.CreateBox(&SphereMaterial, TSphere);
		}
		Scene.CreatePlane(&PlaneMaterial);

		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);
		//Camera.MovementSpeed = 15;
		//Renderer->VisualizePointLightsPositions = true;
		//EController.GetComponent<Components::SpotLightComponent>()->mCastShadows = true;
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
	bool iskinematic = false;

	void Render(float dt) override
	{
		mSceneManager->Update(dt);

		//EController.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera.GetPosition());
	//	EController.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());


		{
			using namespace Graphics;
			ImGui::Begin("Sample3: PhysX & Scripting Test");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Image(ArialFont->mTextureView, {256.f,256.f });

			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);

			static int e = 3;
			ImGui::Text("Rendering Pipelines:");
			ImGui::RadioButton("BlinnPhong", &e, 0);
			ImGui::RadioButton("PBR", &e, 1);

			//Change Rendering Pipeline
			if (e == 0)
				Renderer->SetActiveRenderingPipeline(BlinnPhongPipeline.GetID());
			else if (e == 1)
				Renderer->SetActiveRenderingPipeline(PBRPipeline.GetID());

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

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Engine::GetInstance()->EndClient();
			}

			ImGui::End();
			EntityExplorer(&Scene);
			mDebugSystem->ShowRendertargets();

		}
	}

	void Shutdown() override
	{
		mAssetManager->FlushContainers();
	}
};