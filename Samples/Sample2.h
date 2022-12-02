#pragma once
#include "Common.h"


class Sample2 : public Client
{
	std::shared_ptr<Systems::DebugSystem> mDebugSystem;

	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;

	Assets::Mesh* ShaderBall;

	Assets::MaterialData RustedIron_D;
	Assets::MaterialData Plastic_D;
	Assets::MaterialData Grass_D;
	Assets::MaterialData Gold_D;
	Assets::MaterialData Wall_D;
	
	Assets::Material RustedIron;
	Assets::Material Plastic;
	Assets::Material Grass;
	Assets::Material Gold;
	Assets::Material Wall;

	Graphics::Camera Camera;

	Assets::Shader* PBR;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;
	Rendering::DefferedPass DefferedPass;

	//IBL Settings
	Rendering::ImageBasedLighting IBL;
	Rendering::PBRCapture EnvCapture;
	Graphics::Texture HDREnv;
	Assets::Image HDR_Cube;

	Rendering::Skybox Skybox;
	Assets::Scene Scene;

	ECS::Entity EShaderBall;
	ECS::Entity ESphere;

	ECS::Entity EController;
	ECS::Entity ELights;

	std::vector<ECS::Entity> boxes;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Sample2()
		: Camera(Math::Vector3(0.0f, 5.0f, 30.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, 10.f, Graphics::SENSITIVTY, Graphics::ZOOM)
	{

	}

	void LoadPBRMaterials()
	{
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

		Assets::TextureSet PBRGrass;
		PBRGrass.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/grass/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRGrass.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/grass/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRGrass.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/grass/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRGrass.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/grass/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRGrass.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/grass/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		Grass_D.mTextures.push_back(PBRGrass);
		Grass.SetName("Grass Material");

		Assets::TextureSet PBRWall;
		PBRWall.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/wall/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRWall.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/wall/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRWall.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/wall/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRWall.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/wall/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRWall.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/wall/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		Wall_D.mTextures.push_back(PBRWall);
		Wall.SetName("Wall Material");

		Assets::TextureSet PBRGold;
		PBRGold.mData.push_back({ 0, mAssetManager->Import("@CommonAssets@/Textures/PBR/gold/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRGold.mData.push_back({ 1, mAssetManager->Import("@CommonAssets@/Textures/PBR/gold/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRGold.mData.push_back({ 2, mAssetManager->Import("@CommonAssets@/Textures/PBR/gold/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRGold.mData.push_back({ 3, mAssetManager->Import("@CommonAssets@/Textures/PBR/gold/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRGold.mData.push_back({ 4, mAssetManager->Import("@CommonAssets@/Textures/PBR/gold/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		Gold_D.mTextures.push_back(PBRGold);
		Gold.SetName("Gold Material");

		RustedIron.Create(&RustedIron_D, PBR);
		Plastic.Create(&Plastic_D, PBR);
		Grass.Create(&Grass_D, PBR);
		Wall.Create(&Wall_D, PBR);
		Gold.Create(&Gold_D, PBR);
	}

	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;
		ModelDesc.LoadMaterial = false;
		Assets::MaterialData* temp;
		Assets::Animations* temp2;

		//Load Models
		std::tie(ShaderBall, temp, temp2) = mAssetManager->Import("@CommonAssets@/Models/shaderball/shaderball.obj", ModelDesc);

		LoadPBRMaterials();
	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform shaderballT;
		shaderballT.SetScale(0.5f);

		EShaderBall = Scene.CreateEntity("ShaderBall" , shaderballT);
		ELights = Scene.CreateEntity("Lights");
		EController = Scene.CreateEntity("Controller");

		//Assign Components
		ELights.AddComponent<Components::PointLightComponent>();
		EController.AddComponent<Components::CameraComponent>(&Camera);

		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		ELights.GetComponent<Components::EntityInfoComponent>()->mTransform.SetPosition(Math::Vector3(0.0f, 5.0f, 10.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);

	}
	void InitIBL()
	{
		//IBL
		Importers::ImageLoadingDesc DESC;
		DESC.mType = RESOURCE_DIM_TEX_2D;
		DESC.mUsage = USAGE_IMMUTABLE;
		DESC.mBindFlags = BIND_SHADER_RESOURCE;
		DESC.mMipLevels = 1;

		HDREnv = mAssetManager->Import("@CommonAssets@/Textures/HDR/newport_loft.hdr", DESC, Graphics::TextureUsageType::Unknown);

		Rendering::ImageBasedLightingDesc desc;
		IBL.Initialize(desc);

		HDR_Cube = IBL.EquirectangularToCubemap(&HDREnv);
		EnvCapture = IBL.PrecomputePBRCapture(&HDR_Cube);
		IBL.SetEnvironmentCapture(&EnvCapture);
	}

	void InitRenderer()
	{
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>();

		InitIBL();

		Renderer->AddRenderPass(&GeoPass);
		Renderer->AddRenderPass(&DefferedPass);
		Renderer->AddRenderPass(&PostFXPass);

		Importers::ShaderLoadingDesc desc;
		desc.mType = Importers::ShaderType::_3DRendering;
		PBR = mAssetManager->Import("@NuclearAssets@/Shaders/PBR/PBR.NEShader", desc);

		Renderer->RegisterShader(PBR);
		Renderer->SetIBLContext(&IBL);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);

		PostFXPass.Bake({ _Width_, _Height_,Renderer->mRenderData.mFinalRT.GetDesc()});

		mDebugSystem = Scene.GetSystemManager().Add<Systems::DebugSystem>();
		mDebugSystem->Initialize(&Camera, Renderer->GetAnimationCB());
	}

	void Load()
	{
		mSceneManager->CreateScene(&Scene, true);

		mAssetManager->Initialize();

		mCameraSystem = Scene.GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		SetupEntities();

		InitRenderer();
		ECS::Transform sphere;
		sphere.SetScale(Math::Vector3(5.0f));
		ESphere = Scene.CreateSphere(&RustedIron, sphere);

		SetupAssets();

		EShaderBall.AddComponent<Components::MeshComponent>(ShaderBall, &RustedIron);
		EShaderBall.GetComponent<Components::MeshComponent>()->SetEnableRendering(true);
		ESphere.GetComponent<Components::MeshComponent>()->SetEnableRendering(false);

		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);

		Skybox.Initialize(mCameraSystem->GetCameraCB(),&HDR_Cube);
		PostFXPass.GetBackground().SetSkybox(&Skybox);

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
	}
	bool iskinematic = false;
	bool isDeffered = true;
	void Render(float dt) override
	{
		mSceneManager->Update(dt);
		{	

			using namespace Graphics;
			ImGui::Begin("Sample2: Advanced Rendering");

			static float rotationspeed = 0.0f;
			static Math::Vector3 RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			static ECS::Entity activeentity = EShaderBall;
			ImGui::Text("Model");

			static int f = 0;
			if (ImGui::RadioButton("ShaderBall", &f, 0))
			{
				activeentity = EShaderBall;
				EShaderBall.GetComponent<Components::MeshComponent>()->SetEnableRendering(true);
				ESphere.GetComponent<Components::MeshComponent>()->SetEnableRendering(false);
			}
			if (ImGui::RadioButton("Sphere", &f, 1))
			{
				activeentity = ESphere;
				EShaderBall.GetComponent<Components::MeshComponent>()->SetEnableRendering(false);
				ESphere.GetComponent<Components::MeshComponent>()->SetEnableRendering(true);
			}


			ImGui::Text("Material");

			if (ImGui::Checkbox("Use Deffered Pipeline", &isDeffered))
			{
				ESphere.GetComponent<Components::MeshComponent>()->SetVariantSwitch(Utilities::Hash("NE_DEFFERED"), isDeffered);
				EShaderBall.GetComponent<Components::MeshComponent>()->SetVariantSwitch(Utilities::Hash("NE_DEFFERED"), isDeffered);
			}

			static bool IBL_ = false;

			if (ImGui::Checkbox("IBL", &IBL_))
			{
				ESphere.GetComponent<Components::MeshComponent>()->SetVariantSwitch(Utilities::Hash("IBL_ENABLED"), IBL_);
				EShaderBall.GetComponent<Components::MeshComponent>()->SetVariantSwitch(Utilities::Hash("IBL_ENABLED"), IBL_);
			}
			if (ImGui::Button("Rusted Iron"))
			{
				activeentity.GetComponent<Components::MeshComponent>()->SetMaterial(&RustedIron);
			}
			if (ImGui::Button("Wall"))
			{
				activeentity.GetComponent<Components::MeshComponent>()->SetMaterial(&Wall);
			}
			if (ImGui::Button("Grass"))
			{
				activeentity.GetComponent<Components::MeshComponent>()->SetMaterial(&Grass);
			}
			if (ImGui::Button("Gold"))
			{
				activeentity.GetComponent<Components::MeshComponent>()->SetMaterial(&Gold);
			}
			if (ImGui::Button("Plastic"))
			{
				activeentity.GetComponent<Components::MeshComponent>()->SetMaterial(&Plastic);
			}


			ImGui::SliderFloat("Rotation Speed", &rotationspeed, 0.0f, 2.0f);

			ImGui::SliderFloat3("Rotation Axis", (float*)&RotationAxis, 0.0f, 1.0f);

			float rotationAngle = LastFrame / 5.0f * rotationspeed;
			activeentity.GetComponent<Components::EntityInfoComponent>()->mTransform.SetRotation(RotationAxis, rotationAngle);

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


			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Checkbox("Visualize RenderTargets", &mDebugSystem->ShowRegisteredRenderTargets);

			ImGui::Checkbox("Visualize LightSources", &mDebugSystem->RenderLightSources);

			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);

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