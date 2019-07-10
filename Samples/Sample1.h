#pragma once
#include "Common.h"


void ViewMaterialInfo(Assets::Material* material, Managers::AssetManager* Manager)
{
	using namespace Graphics;
	std::string name = Manager->mHashedMaterialsPaths[material->GetName()].mInputPath + Utilities::int_to_hex<Uint32>(material->GetName());

	if (Manager)
		name = Manager->mHashedMaterialsPaths[material->GetName()].mInputPath + Utilities::int_to_hex<Uint32>(material->GetName());
	else
		name = Utilities::int_to_hex<Uint32>(material->GetName()).c_str();

	ImGui::Begin(name.c_str());

	for (int i = 0; i < material->mPixelShaderTextures.size(); i++)
	{
		ImGui::Text(std::string("TextureSet Index: " + std::to_string(i)).c_str());
		for (int j = 0; j < material->mPixelShaderTextures.at(i).mData.size(); j++)
		{
			ImGui::Image(&material->mPixelShaderTextures.at(i).mData.at(j).mTex, ImVec2(128, 128));
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}
	ImGui::End();
}
class Sample1 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;

	//Asset Manager (Loader)
	Managers::AssetManager AssetLoader;
	Managers::CameraManager SceneCameraManager;

	Assets::Mesh* NanosuitAsset;
	Assets::Mesh* CyborgAsset;

	Assets::Material CubeMaterial;
	Assets::Material* NanosuitMaterial;
	Assets::Material* CyborgMaterial;

	Components::CameraComponent Camera;

	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::PointLight pointlight5;
	Components::PointLight pointlight6;
	Components::PointLight pointlight7;
	Components::PointLight pointlight8;
	Components::PointLight pointlight9;

	Components::SpotLight spotLight;

	Graphics::Skybox Skybox;

	Graphics::DiffuseOnly DiffuseRP;
	Graphics::WireFrame WireFrameRP;
	Graphics::BlinnPhong BlinnPhongRP;
	Graphics::BlinnPhong BlinnPhongWithNormalMapRP = Graphics::BlinnPhong(true);

	//ECS
	ECS::Scene ModelsScene;
	ECS::Entity ESkybox;
	ECS::Entity ECube;
	ECS::Entity ECyborg;
	ECS::Entity ENanosuit;

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
	bool renderSkybox = true;

	void SetupLights()
	{
		dirlight.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dirlight.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));

		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight3.SetPosition(pointLightPositions[2]);
		pointlight3.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight4.SetPosition(pointLightPositions[3]);
		pointlight4.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight5.SetPosition(pointLightPositions[4]);
		pointlight5.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight6.SetPosition(pointLightPositions[5]);
		pointlight6.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight7.SetPosition(pointLightPositions[6]);
		pointlight7.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight8.SetPosition(pointLightPositions[7]);
		pointlight8.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight9.SetPosition(pointLightPositions[8]);
		pointlight9.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));
	}
	void SetupAssets()
	{

		Importers::MeshLoadingDesc ModelDesc;

		//Load Nanosuit Model
		std::tie(NanosuitAsset, NanosuitMaterial) = AssetLoader.Import("@CommonAssets@/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);
		//Load Cyborg Model
		std::tie(CyborgAsset, CyborgMaterial) = AssetLoader.Import("@CommonAssets@/Models/CrytekCyborg/cyborg.obj", ModelDesc);

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, AssetLoader.Import("@CommonAssets@/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse) });
		CubeSet.mData.push_back({ 1, AssetLoader.Import("@CommonAssets@/Textures/crate_specular.png", Assets::TextureUsageType::Specular) });
		CubeSet.mData.push_back({ 2, AssetLoader.Import("@CommonAssets@/Textures/crate_normal.png", Assets::TextureUsageType::Normal) });

		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		Renderer->CreateMaterialForAllPipelines(&CubeMaterial);
		Renderer->CreateMaterialForAllPipelines(NanosuitMaterial);
		Renderer->CreateMaterialForAllPipelines(CyborgMaterial);

		CubeSet.mData.clear();

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
		Skybox.Initialize(SceneCameraManager.GetCameraCB(), AssetLoader.LoadTextureCubeFromFile(SkyBoxTexturePaths, SkyboxDesc));
	}
	void SetupEntities()
	{
		//Create Entities
		ECube = ModelsScene.CreateEntity();
		ENanosuit = ModelsScene.CreateEntity();
		ECyborg = ModelsScene.CreateEntity();

		//Assign Components
		ECube.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(), &CubeMaterial);
		ENanosuit.Assign<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
		ECyborg.Assign<Components::MeshComponent>(CyborgAsset, CyborgMaterial);
	}

	void InitRenderer()
	{
		Renderer = ModelsScene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		ModelsScene.Systems.Configure();

		Renderer->AddRenderingPipeline(&BlinnPhongRP);
		Renderer->AddRenderingPipeline(&BlinnPhongWithNormalMapRP);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);

		Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);
		Renderer->AddLight(&pointlight5);
		Renderer->AddLight(&pointlight6);
		Renderer->AddLight(&pointlight7);
		Renderer->AddLight(&pointlight8);
		Renderer->AddLight(&pointlight9);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();
	}

	void Load()
	{
		Assets::DefaultTextures::Initalize(&AssetLoader);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		InitRenderer();

		SetupLights();

		SetupAssets();

		SetupEntities();

		//Setup positions
		Math::Matrix4 TNanosuit(1.0f);
		TNanosuit = Math::translate(TNanosuit, Math::Vector3(0.0f, -1.75f, 0.0f));
		TNanosuit = Math::scale(TNanosuit, Math::Vector3(0.3f, 0.3f, 0.3f));
		ENanosuit.GetComponent<Components::TransformComponent>()->SetTransform(TNanosuit);

		Math::Matrix4 TCyborg(1.0f);
		TCyborg = Math::translate(TCyborg, Math::Vector3(4.0f, -1.75f, 0.0f));
		ECyborg.GetComponent<Components::TransformComponent>()->SetTransform(TCyborg);

		Math::Matrix4 TCube(1.0f);
		TCube = Math::translate(TCube, Math::Vector3(2.0f, -1.75f, 2.0f));
		ECube.GetComponent<Components::TransformComponent>()->SetTransform(TCube);

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
	void Update(float deltatime) override
	{
		//Movement
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

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
		Graphics::Context::GetContext()->ClearRenderTarget(nullptr, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(nullptr, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		Renderer->Update(ModelsScene.Entities, ModelsScene.Events, dt);

		if (renderSkybox)
			Skybox.Render();

		{
			using namespace Graphics;
			ImGui::Begin("Sample1: Basic Rendering");

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

			ImGui::Checkbox("Render Skybox", &renderSkybox);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};