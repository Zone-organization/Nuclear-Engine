#pragma once
#include "Common.h"

class Sample3 : public Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;
	std::shared_ptr<Systems::ScriptingSystem> ScriptSystem;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
	std::shared_ptr<Systems::LightingSystem> mLightingSystem;

	Assets::Material PlaneMaterial;
	Assets::Material CubeMaterial;
	Graphics::Camera Camera;

	Rendering::DiffuseOnly DiffuseRP;
	Rendering::ForwardRenderingPipeline DiffuseRPPipeline;
	ECS::Scene Scene;

	ECS::Entity EPlane;
	Assets::Mesh gPlane;
	Assets::Script script;

	ECS::Entity EScriptOwner;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Sample3()
		: Camera(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, Graphics::SPEED, Graphics::SENSITIVTY, Graphics::ZOOM)
	, DiffuseRPPipeline("Diffuse")
	{
	}

	void SetupEntities()
	{
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/crate_diffuse.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });

		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);


		Assets::TextureSet PlaneSet;
		PlaneSet.mData.push_back({ 0, Managers::AssetManager::DefaultGreyTex });

		PlaneMaterial.mPixelShaderTextures.push_back(PlaneSet);

		//Create Entities
		EPlane = Scene.CreateEntity();

		Assets::Mesh::CreatePlane(&gPlane, 100.0f, 100.0f);

		//Assign Components
		EPlane.AddComponent<Components::MeshComponent>(&gPlane, &PlaneMaterial);

		Components::ColliderDesc desc;
		desc.mShape = Components::ColliderShape::Plane;
		auto geo = PhysX::PlaneGeometry(ECS::Transform());
		desc.mGeo = &geo;
		EPlane.AddComponent<Components::ColliderComponent>(desc);

		mPhysXSystem->AddActor(EPlane);

		EScriptOwner = Scene.GetFactory().CreateBox(&CubeMaterial,ECS::Transform());
		auto sc = EScriptOwner.AddComponent<Components::ScriptComponent>();
		sc.script = &script;

		Renderer->CreateMaterialForAllPipelines(&CubeMaterial);
		Renderer->CreateMaterialForAllPipelines(&PlaneMaterial);
	}
	void SetupSystems()
	{
		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();
		ScriptSystem = Scene.GetSystemManager().Add<Systems::ScriptingSystem>();
		mLightingSystem = Scene.GetSystemManager().Add<Systems::LightingSystem>();
		mLightingSystem->Bake();

		Systems::PhysXSystemDesc sceneDesc;
		sceneDesc.mGravity = Math::Vector3(0.0f, -9.81f, 0.0f);
		mPhysXSystem = Scene.GetSystemManager().Add<Systems::PhysXSystem>(sceneDesc);

		ScriptSystem->Initialize();
		DiffuseRPPipeline.Initialize(&DiffuseRP, &Camera);
		Renderer->AddRenderingPipeline(&DiffuseRPPipeline);
		Renderer->Bake(_Width_, _Height_);
		Renderer->SetActiveRenderingPipeline(DiffuseRPPipeline.GetID());
	}

	void Load() override
	{
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));

		SetupSystems();

		SetupEntities();


		script.SetName("Script1");
		Scripting::ScriptingModule* smodule;

		smodule = ScriptSystem->CreateScriptingModule({ "Main" , Scripting::ScriptModuleCreationFlags::ALWAYS_CREATE });

		ScriptSystem->GetScriptingEngine()->CreateScript(&script, Core::FileSystem::LoadFileToString("Assets/script.as"), smodule);
		ScriptSystem->GetScriptingEngine()->BuildScriptingModule(smodule);

		ScriptSystem->Load();

		Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
	}


	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Renderer->ResizeRenderTargets(width, height);
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
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.3f,  0.3f,  0.3f, 1.0f };

		Scene.Update(dt);


		if (Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_SPACE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			//auto ECube = Scene.GetFactory().CreateBox(&CubeMaterial, ECS::Transform(Camera.GetPosition(), Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f)));
		//	ECube.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Camera.GetPosition());
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
				return Engine::GetInstance()->EndGame();
			}
			ImGui::End();

		}
	}
};