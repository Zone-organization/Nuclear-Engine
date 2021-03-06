#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;
	//Asset Manager (Loader)
	Managers::AssetManager AssetLoader;
	Managers::CameraManager SceneCameraManager;

	Assets::Mesh* SponzaAsset;
	Assets::Material* SponzaMaterial;

	Assets::Mesh* CerberusAsset;
	Assets::Material* CerberusMaterial;

	Assets::Material SphereMaterial;

	Components::CameraComponent Camera;

	Graphics::PBR PBR;
	Graphics::BlinnPhong BlinnPhong;
	Graphics::DiffuseOnly DiffuseRP;
	Graphics::WireFrame WireFrameRP;

	//ECS
	ECS::Scene PBRScene;
	ECS::Entity ESponza;
	ECS::Entity ESphere;
	ECS::Entity ECerberus;

	ECS::Entity ECamera;
	ECS::Entity ELights;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;

		//Load Models
		std::tie(SponzaAsset, SponzaMaterial) = AssetLoader.Import("Assets/Common/Models/CrytekSponza/sponza.obj", ModelDesc);
		std::tie(CerberusAsset, CerberusMaterial) = AssetLoader.Import("Assets/Common/Models/Cerberus/Cerberus.FBX", ModelDesc);

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRSphereSet;
		PBRSphereSet.mData.push_back({ 0, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/albedo.png", Assets::TextureUsageType::Diffuse) });
		PBRSphereSet.mData.push_back({ 1, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/metallic.png", Assets::TextureUsageType::Specular) });
		PBRSphereSet.mData.push_back({ 2, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/normal.png", Assets::TextureUsageType::Normal) });
		PBRSphereSet.mData.push_back({ 3, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Assets::TextureUsageType::Roughness) });
		PBRSphereSet.mData.push_back({ 4, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/ao.png", Assets::TextureUsageType::AO) });

		SphereMaterial.mPixelShaderTextures.push_back(PBRSphereSet);
		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
		Renderer->CreateMaterialForAllPipelines(SponzaMaterial);
		Renderer->CreateMaterialForAllPipelines(CerberusMaterial);

		ESphere.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), &SphereMaterial);
		ESponza.Assign<Components::MeshComponent>(SponzaAsset, SponzaMaterial);
		ECerberus.Assign<Components::MeshComponent>(CerberusAsset, CerberusMaterial);

		PBRSphereSet.mData.clear();
	}
	void SetupEntities()
	{
		//Create Entities
		ESphere = PBRScene.CreateEntity();
		ESponza = PBRScene.CreateEntity();
		ELights = PBRScene.CreateEntity();
		ECamera = PBRScene.CreateEntity();
		ECerberus = PBRScene.CreateEntity();

		//Assign Components
	
		ELights.Assign<Components::DirLightComponent>();
		ELights.Assign<Components::PointLightComponent>();
		ECamera.Assign<Components::SpotLightComponent>();

		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::PointLightComponent>()->SetPosition(Math::Vector3(0.0f, 0.0f, 10.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);

	}
	void InitRenderer()
	{
		Renderer = PBRScene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		PBRScene.Systems.Configure();
		Renderer->AddRenderingPipeline(&PBR);
		Renderer->AddRenderingPipeline(&BlinnPhong);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);

		Renderer->Bake(PBRScene.Entities);
	}

	void Load()
	{
		Assets::DefaultTextures::Initalize(&AssetLoader);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		SetupEntities();

		InitRenderer();

		SetupAssets();

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		Math::Matrix4 model;
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = Math::Matrix4(1.0);
				model = Math::translate(model, Math::Vector3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f
				));
				model = Math::scale(model, Math::Vector3(2.0f));

				ESphere.GetComponent<Components::MeshComponent>()->mMultiRenderTransforms.push_back(model);
			}
		}

		ESphere.GetComponent<Components::MeshComponent>()->mMultiRender = true;


		Math::Matrix4 TSponza(1.0f);
		TSponza = Math::scale(TSponza, Math::Vector3(0.05f));
		ESponza.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TSponza);

		Math::Matrix4 TCerberus(1.0f);
		TCerberus = Math::scale(TCerberus, Math::Vector3(0.05f));
		ECerberus.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TCerberus);

		Components::CameraBakingOptions Desc;
		Desc.RTWidth = _Width_;
		Desc.RTHeight = _Height_;
		Desc.Disable_Bloom_Varient = true;
		Camera.Bake(Desc);

		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);


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

		Camera.UpdatePSO();
	}
	void Render(float dt) override
	{
		ECamera.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera.GetPosition());
		ECamera.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());
		
		Renderer->Update(PBRScene.Entities, PBRScene.Events, dt);

		{
			using namespace Graphics;
			ImGui::Begin("Sample2: Advanced Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
			if (ImGui::TreeNode("Rendering"))
			{
				ImGui::Text("Active Rendering Pipeline:");
				static int e = 0;
				ImGui::RadioButton("PBR", &e, 0);
				ImGui::RadioButton("BlinnPhong", &e, 1);
				ImGui::RadioButton("DiffuseOnly", &e, 2);
				ImGui::RadioButton("WireFrame", &e, 3);

				//Change Rendering Pipeline
				if (e == 0)
					Renderer->SetActiveRenderingPipeline(PBR.GetID());
				else if (e == 1)
					Renderer->SetActiveRenderingPipeline(BlinnPhong.GetID());
				else if (e == 2)
					Renderer->SetActiveRenderingPipeline(DiffuseRP.GetID());
				else if (e == 3)
					Renderer->SetActiveRenderingPipeline(WireFrameRP.GetID());

				static int er = 0;
				ImGui::RadioButton("Spheres", &er, 0);
				ImGui::RadioButton("Sponza", &er, 1);
				ImGui::RadioButton("Cerberus", &er, 2);

				switch (er)
				{
				case 0:
					ESponza.GetComponent<Components::MeshComponent>()->mRender = false;
					ESphere.GetComponent<Components::MeshComponent>()->mRender = true;
					ECerberus.GetComponent<Components::MeshComponent>()->mRender = false;
					break;
				case 1:
					ESponza.GetComponent<Components::MeshComponent>()->mRender = true;
					ESphere.GetComponent<Components::MeshComponent>()->mRender = false;
					ECerberus.GetComponent<Components::MeshComponent>()->mRender = false;
					break;
				case 2:
					ESponza.GetComponent<Components::MeshComponent>()->mRender = false;
					ESphere.GetComponent<Components::MeshComponent>()->mRender = false;
					ECerberus.GetComponent<Components::MeshComponent>()->mRender = true;
					break;
				default:
					break;
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Lights"))
			{
				static ImVec4 Lightcolor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
				static float f = 1.0f;
				ImGui::SliderFloat("PointLight Intensity", &f, 0.0f, 25.0f, "%.4f", 2.0f);
				ImGui::ColorEdit3("PointLight Color", (float*)& Lightcolor);
				ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);

				ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(Lightcolor.x, Lightcolor.y, Lightcolor.z, Lightcolor.w));
				ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(f);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Camera"))
			{
				ImGui::ColorEdit3("RenderTarget ClearColor", (float*)& Camera.RTClearColor);
				ImGui::Checkbox("HDR", &Camera.HDR);
				ImGui::Checkbox("GammaCorrection", &Camera.GammaCorrection);

				ImGui::TreePop();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};