#pragma once
#include "Common.h"

class Sample3 : public Client
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;
	std::shared_ptr<Systems::ScriptingSystem> mScriptSystem;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;

	Assets::MaterialData RustedIron_D;
	Assets::MaterialData Plastic_D;

	Assets::Material RustedIron;
	Assets::Material Plastic;

	Graphics::Camera Camera;

	Assets::Script* script;
	Assets::Shader* PBR;
	Rendering::ForwardRenderingPath ForwardRP;

	Assets::Scene Scene;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;

	ECS::Entity EController;
	ECS::Entity ELights;

	std::vector<ECS::Entity> boxes;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Sample3()
		: Camera(Math::Vector3(0.0f, 5.0f, 30.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, 10.f, Graphics::SENSITIVTY, Graphics::ZOOM)
	{

	}
	void SetupAssets()
	{
		script = mAssetManager->Import("@CurrentPath@/../Textures/SamplesScripts/Sample3.cs", Importers::ScriptImportingDesc());

		//Initialize Materials
		Assets::TextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRRustedIron.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRRustedIron.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRRustedIron.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRRustedIron.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		RustedIron_D.mTextures.push_back(PBRRustedIron);
		RustedIron.SetName("RustedIron Material");

		Assets::TextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRPlastic.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRPlastic.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRPlastic.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRPlastic.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		Plastic_D.mTextures.push_back(PBRPlastic);
		Plastic.SetName("Plastic Material");

		RustedIron.Create(&RustedIron_D, PBR);
		Plastic.Create(&Plastic_D, PBR);
	}
	void SetupEntities()
	{
		//Create Entities
		ELights = Scene.CreateEntity("Lights");
		EController = Scene.CreateEntity("Controller");

		//Assign Components
		ELights.AddComponent<Components::DirLightComponent>();
		ELights.AddComponent<Components::PointLightComponent>();
		EController.AddComponent<Components::SpotLightComponent>();
		EController.AddComponent<Components::CameraComponent>(&Camera);

		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));

		ELights.GetComponent<Components::DirLightComponent>().SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>().SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(0.0f, 5.0f, 10.0f));
		ELights.GetComponent<Components::PointLightComponent>().SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>().SetIntensity(10.0f);

	}
	void InitRenderer()
	{
		Systems::PhysXSystemDesc sceneDesc;
		sceneDesc.mGravity = Math::Vector3(0.0f, -7.0f, 0.0f);
		mPhysXSystem = Scene.GetSystemManager().Add<Systems::PhysXSystem>(sceneDesc);

		mScriptSystem = Scene.GetSystemManager().Add<Systems::ScriptingSystem>();
		mScriptSystem->Initialize();
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();
		Renderer->AddRenderPass(&GeoPass);
		Renderer->AddRenderPass(&PostFXPass);

		Importers::ShaderLoadingDesc desc;
		desc.mType = Importers::ShaderType::_3DRendering;
		PBR = mAssetManager->Import("@NuclearAssets@/Shaders/PBR/PBR.NEShader", desc);

		Renderer->RegisterShader(PBR);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);

		PostFXPass.Bake({ _Width_, _Height_,Renderer->mRenderData.mFinalRT.GetDesc() });
	}

	void Load()
	{
		mSceneManager->CreateScene(&Scene, true);

		mAssetManager->Initialize();

		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);

		SetupEntities();

		InitRenderer();

		SetupAssets();

		auto entity = Scene.CreateEntity("ScriptEntity");
		auto& comp = entity.AddComponent<Components::ScriptComponent>(script);

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				Math::Vector3 position(
					(float)(col - (nrColumns / 2)) * spacing,
					((float)(row - (nrRows / 2)) * spacing) + 10.0f,
					0.0f
				);


				ECS::Transform ESphere(position, Math::Vector3(2.0f));

				auto sphere = Scene.CreateSphere(&RustedIron, ESphere);
				position.z += 5.0f;

				//ECS::Transform EBox(position, Math::Vector3(1.0f));

				//boxes.push_back(Scene.GetFactory().CreateBox(&SphereMaterial, EBox));
			}
		}

		Scene.CreatePlane(&Plastic);
		for (auto it : boxes)
		{
			it.GetComponent<Components::RigidBodyComponent>().isKinematic = true;
		}

		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);
		//Scene.Save();
		//Camera.MovementSpeed = 15;
		//Renderer->VisualizePointLightsPositions = true;




		Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
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
		Graphics::Context::GetInstance().GetSwapChain()->Resize(width, height);
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Renderer->ResizeRTs(width, height);
	}

	void Update(float deltatime) override
	{
		//Movement
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_RIGHT, deltatime);

		//Change Mouse Mode
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera.UpdateBuffer();
		mCameraSystem->Update(deltatime);

		EController.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Camera.GetPosition());

	}
	bool iskinematic = false;

	void Render(float dt) override
	{
		mSceneManager->Update(dt);

		EController.GetComponent<Components::SpotLightComponent>().SetDirection(Camera.GetFrontView());


		{
			using namespace Graphics;
			ImGui::Begin("Sample3: PhysX & Scripting Test");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
		
			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);
									
			PhysX::RaycastHit hit;
			if (ImGui::TreeNode("Raycast Info"))
			{
				if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_F) == Core::Input::KeyboardKeyStatus::Pressed)
				{

					if (mPhysXSystem->Raycast(Camera.GetPosition(), Camera.GetFrontView(), 100.f, hit))
					{
						auto entity = hit.HitEntity;

						ImGui::Text((char*)Scene.GetRegistry().try_get<Components::EntityInfoComponent>(entity.GetEntityID())->mName.c_str());
					}
					else
					{
						ImGui::Text("No hit");
					}
				}
				else
					ImGui::Text("Press F");

				ImGui::TreePop();
			}

			ImGui::Separator();
			ImGui::Text("PostFX Pipeline");

			if (ImGui::TreeNode("PostFX Effects"))
			{
				for (auto& it : PostFXPass.GetPipelineController().GetSwitches())
				{
					bool value = it.second.GetValue();
					ImGui::Checkbox(it.second.GetName().c_str(), &value);
					if (value != it.second.GetValue())
					{
						PostFXPass.SetPostProcessingEffect(it.second.GetID(), value);
					}
				}
				ImGui::TreePop();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Engine::GetInstance().EndClient();
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