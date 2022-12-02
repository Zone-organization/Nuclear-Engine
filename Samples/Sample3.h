//#pragma once
//#include "Common.h"
//
//class Sample3 : public Client
//{
//	std::shared_ptr<Systems::RenderSystem> Renderer;
//	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;
//	std::shared_ptr<Systems::ScriptingSystem> ScriptSystem;
//	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
//	std::shared_ptr<Systems::LightingSystem> mLightingSystem;
//
//	Assets::Material SphereMaterial;
//	Assets::Material PlaneMaterial;
//
//	Graphics::Camera Camera;
//
//	Rendering::PBR PBR;
//	Rendering::PBR DefferedPBR;
//
//	Rendering::DiffuseOnly DiffuseRP;
//	Rendering::WireFrame WireFrameRP;
//
//	Rendering::ForwardRenderingPipeline PBRPipeline;
//	Rendering::ForwardRenderingPipeline DiffuseRPPipeline;
//	Rendering::ForwardRenderingPipeline WireFrameRPPipeline;
//
//	Rendering::DefferedRenderingPipeline PBRDefferedPipeline;
//
//
//	Assets::Scene Scene;
//
//	ECS::Entity EController;
//	ECS::Entity ELights;
//
//	std::vector<ECS::Entity> boxes;
//
//	float lastX = _Width_ / 2.0f;
//	float lastY = _Height_ / 2.0f;
//	bool firstMouse = true;
//	bool isMouseDisabled = false;
//public:
//	Sample3()
//		: Camera(Math::Vector3(0.0f, 5.0f, 30.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, 10.f, Graphics::SENSITIVTY, Graphics::ZOOM)
//	{
//
//	}
//	void SetupAssets()
//	{
//		//Load some textures manually
//		Importers::ImageLoadingDesc desc;
//		//desc.mFormat = TEX_FORMAT_RGBA8_UNORM;
//
//		//Initialize Materials
//		Assets::TextureSet PBRSphereSet;
//		PBRSphereSet.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/albedo.png",desc, Graphics::TextureUsageType::Diffuse) });
//		PBRSphereSet.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/metallic.png", desc,Graphics::TextureUsageType::Specular) });
//		PBRSphereSet.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/normal.png",desc, Graphics::TextureUsageType::Normal) });
//		PBRSphereSet.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/roughness.png", desc, Graphics::TextureUsageType::Roughness) });
//		PBRSphereSet.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/RustedIron/ao.png", desc, Graphics::TextureUsageType::AO) });
//
//		SphereMaterial.mPixelShaderTextures.push_back(PBRSphereSet);
//		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
//
//		Assets::TextureSet PBRPlaneSet;
//		PBRPlaneSet.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/albedo.png",desc, Graphics::TextureUsageType::Diffuse) });
//		PBRPlaneSet.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/metallic.png", desc,Graphics::TextureUsageType::Specular) });
//		PBRPlaneSet.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/normal.png",desc, Graphics::TextureUsageType::Normal) });
//		PBRPlaneSet.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/roughness.png", desc, Graphics::TextureUsageType::Roughness) });
//		PBRPlaneSet.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/plastic/ao.png", desc, Graphics::TextureUsageType::AO) });
//
//		PlaneMaterial.mPixelShaderTextures.push_back(PBRPlaneSet);
//		PlaneMaterial.SetName("Plane Material");
//		Renderer->CreateMaterialForAllPipelines(&PlaneMaterial);
//
//		PBRSphereSet.mData.clear();
//	}
//	void SetupEntities()
//	{
//		//Create Entities
//		ELights = Scene.CreateEntity("Lights");
//		EController = Scene.CreateEntity("Controller");
//
//		//Assign Components
//		ELights.AddComponent<Components::DirLightComponent>();
//		ELights.AddComponent<Components::PointLightComponent>();
//		EController.AddComponent<Components::SpotLightComponent>();
//		EController.AddComponent<Components::CameraComponent>(&Camera);
//
//		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
//
//		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
//		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));
//
//		ELights.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Math::Vector3(0.0f, 5.0f, 10.0f));
//		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
//		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);
//
//	}
//	void InitRenderer()
//	{
//		Systems::PhysXSystemDesc sceneDesc;
//		sceneDesc.mGravity = Math::Vector3(0.0f, -7.0f, 0.0f);
//		mPhysXSystem = Scene.GetSystemManager().Add<Systems::PhysXSystem>(sceneDesc);
//
//		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();
//
//		PBRPipeline.Initialize(&PBR, &Camera);
//
//		DiffuseRPPipeline.Initialize(&DiffuseRP, &Camera);
//		WireFrameRPPipeline.Initialize(&WireFrameRP, &Camera);
//		DefferedPBR.Initialize({ true });
//
//		Rendering::DefferedRenderingPipelineInitInfo initInfo;
//		initInfo.camera = &Camera;
//		initInfo.shadingModel = &DefferedPBR;
//		PBRDefferedPipeline.Initialize(initInfo);
//
//		//Scene.Systems.Configure();
//		//TestPBR.test = true;
//		Renderer->AddRenderingPipeline(&PBRPipeline);
//		Renderer->AddRenderingPipeline(&PBRDefferedPipeline);
//		Renderer->AddRenderingPipeline(&DiffuseRPPipeline);
//		Renderer->AddRenderingPipeline(&WireFrameRPPipeline);
//
//		Renderer->Bake(_Width_, _Height_);
//	}
//
//	void Load()
//	{
//		mSceneManager->CreateScene(&Scene, true);
//
//		mAssetManager->Initialize();
//
//		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);
//		mLightingSystem = Scene.GetSystemManager().Add<Systems::LightingSystem>();
//		SetupEntities();
//		mLightingSystem->Bake();
//
//		InitRenderer();
//
//		SetupAssets();
//
//		int nrRows = 7;
//		int nrColumns = 7;
//		float spacing = 2.5;
//		for (int row = 0; row < nrRows; ++row)
//		{
//			for (int col = 0; col < nrColumns; ++col)
//			{
//				Math::Vector3 position(
//					(float)(col - (nrColumns / 2)) * spacing,
//					((float)(row - (nrRows / 2)) * spacing) + 10.0f,
//					0.0f
//				);
//
//
//				ECS::Transform ESphere(position, Math::Vector3(2.0f));
//
//				auto sphere = Scene.CreateSphere(&SphereMaterial, ESphere);
//				position.z += 5.0f;
//
//				//ECS::Transform EBox(position, Math::Vector3(1.0f));
//
//				//boxes.push_back(Scene.GetFactory().CreateBox(&SphereMaterial, EBox));
//			}
//		}
//
//		Scene.CreatePlane(&PlaneMaterial);
//		for (auto it : boxes)
//		{
//			it.GetComponent<Components::RigidBodyComponent>()->isKinematic = true;
//		}
//
//		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);
//		//Scene.Save();
//		//Camera.MovementSpeed = 15;
//		//Renderer->VisualizePointLightsPositions = true;
//		Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
//	}
//	void OnMouseMovement(int xpos_a, int ypos_a) override
//	{
//		if (!isMouseDisabled)
//		{
//			float xpos = static_cast<float>(xpos_a);
//			float ypos = static_cast<float>(ypos_a);
//
//			if (firstMouse)
//			{
//				lastX = xpos;
//				lastY = ypos;
//				firstMouse = false;
//			}
//
//			float xoffset = xpos - lastX;
//			float yoffset = lastY - ypos;
//
//			lastX = xpos;
//			lastY = ypos;
//
//			Camera.ProcessEye(xoffset, yoffset);
//		}
//	}
//	void OnWindowResize(int width, int height) override
//	{
//		Graphics::Context::GetInstance().GetSwapChain()->Resize(width, height);
//		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
//		Renderer->ResizeRenderTargets(width, height);
//	}
//
//	void Update(float deltatime) override
//	{
//		//Movement
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
//			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_FORWARD, deltatime);
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
//			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_LEFT, deltatime);
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
//			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_BACKWARD, deltatime);
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
//			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_RIGHT, deltatime);
//
//		//Change Mouse Mode
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
//		{
//			isMouseDisabled = true;
//			Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
//		}
//		if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
//		{
//			isMouseDisabled = false;
//			Engine::GetInstance().GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
//		}
//
//		Camera.UpdateBuffer();
//		mCameraSystem->Update(deltatime);
//
//		EController.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Camera.GetPosition());
//
//		Renderer->GetActivePipeline()->UpdatePSO();
//	}
//	bool iskinematic = false;
//
//	void Render(float dt) override
//	{
//		mSceneManager->Update(dt);
//
//		EController.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera.GetFrontView());
//
//
//		{
//			using namespace Graphics;
//			ImGui::Begin("Sample3: PhysX & Scripting Test");
//
//			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
//			if (ImGui::TreeNode("Rendering"))
//			{
//				ImGui::Text("Active Rendering Pipeline:");
//				static int e = 0;
//				ImGui::RadioButton("PBR", &e, 0);
//				ImGui::RadioButton("Deffered PBR", &e, 1);
//				ImGui::RadioButton("DiffuseOnly", &e, 2);
//				ImGui::RadioButton("WireFrame", &e, 3);
//
//				//Change Rendering Pipeline
//				if (e == 0)
//					Renderer->SetActiveRenderingPipeline(PBRPipeline.GetID());
//				else if (e == 1)
//					Renderer->SetActiveRenderingPipeline(PBRDefferedPipeline.GetID());
//				else if (e == 2)
//					Renderer->SetActiveRenderingPipeline(DiffuseRPPipeline.GetID());
//				else if (e == 3)
//					Renderer->SetActiveRenderingPipeline(WireFrameRPPipeline.GetID());
//
//				//ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);
//
//				ImGui::TreePop();
//			}
//
//
//			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);
//
//
//			if (ImGui::TreeNode("Pipeline Effects"))
//			{
//				for (auto& it : Renderer->GetActivePipeline()->mPairedEffects)
//				{
//					bool value = it.second.GetValue();
//					ImGui::Checkbox(it.second.GetName().c_str(), &value);
//					if (value != it.second.GetValue())
//					{
//						Renderer->GetActivePipeline()->SetEffect(it.second.GetID(), value);
//					}
//				}
//				ImGui::TreePop();
//			}
//
//			PhysX::RaycastHit hit;
//			if (ImGui::TreeNode("Raycast Info"))
//			{
//				if (Engine::GetInstance().GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_F) == Core::Input::KeyboardKeyStatus::Pressed)
//				{
//
//					if (mPhysXSystem->Raycast(Camera.GetPosition(), Camera.GetFrontView(), 100.f, hit))
//					{
//						auto entity = hit.HitEntity;
//
//						ImGui::Text((char*)Scene.GetRegistry().try_get<Components::EntityInfoComponent>(entity.entity)->mName.c_str());
//					}
//					else
//					{
//						ImGui::Text("No hit");
//					}
//				}
//				else
//					ImGui::Text("Press F");
//
//				ImGui::TreePop();
//			}
//
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//			if (ImGui::Button("End Game"))
//			{
//				ImGui::End();
//				return Engine::GetInstance().EndClient();
//			}
//
//			ImGui::End();
//			EntityExplorer(&Scene);
//		}
//	}
//
//	void Shutdown() override
//	{
//		mAssetManager->FlushContainers();
//	}
//};