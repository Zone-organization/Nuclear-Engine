#pragma once
#include "Common.h"


//Current TODO:
//Shadow test
class Playground : public Core::Client
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::CameraSystem> mCameraSystem;
	std::shared_ptr<Systems::DebugSystem> mDebugSystem;

	Assets::Material RustedIron;
	Assets::Material Plastic;

	Assets::MaterialData RustedIron_D;
	Assets::MaterialData Plastic_D;

	Assets::Font* ArialFont;

	Graphics::Camera Camera;

	Assets::Shader* PBR;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;
	//Rendering::DefferedPass DefferedPass;
	Rendering::ShadowPass ShadowPass;

	ECS::Entity EController;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Playground()
		: Camera(Math::Vector3(0.0f, 5.0f, 30.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Graphics::YAW, Graphics::PITCH, 10.f, Graphics::SENSITIVTY, Graphics::ZOOM)
	{
	}
	void SetupAssets()
	{
		//Load some textures manually
		Importers::ImageLoadingDesc desc;
		//desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ 0, GetAssetManager().Import("@CommonAssets@/Textures/PBR/RustedIron/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRRustedIron.mData.push_back({ 1, GetAssetManager().Import("@CommonAssets@/Textures/PBR/RustedIron/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRRustedIron.mData.push_back({ 2, GetAssetManager().Import("@CommonAssets@/Textures/PBR/RustedIron/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRRustedIron.mData.push_back({ 3, GetAssetManager().Import("@CommonAssets@/Textures/PBR/RustedIron/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRRustedIron.mData.push_back({ 4, GetAssetManager().Import("@CommonAssets@/Textures/PBR/RustedIron/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		RustedIron_D.mTextures.push_back(PBRRustedIron);
		RustedIron.SetName("RustedIron Material");

		Assets::TextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ 0, GetAssetManager().Import("@CommonAssets@/Textures/PBR/plastic/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRPlastic.mData.push_back({ 1, GetAssetManager().Import("@CommonAssets@/Textures/PBR/plastic/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRPlastic.mData.push_back({ 2, GetAssetManager().Import("@CommonAssets@/Textures/PBR/plastic/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRPlastic.mData.push_back({ 3, GetAssetManager().Import("@CommonAssets@/Textures/PBR/plastic/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRPlastic.mData.push_back({ 4, GetAssetManager().Import("@CommonAssets@/Textures/PBR/plastic/ao.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		Plastic_D.mTextures.push_back(PBRPlastic);
		Plastic.SetName("Plastic Material");

		RustedIron.Create(&RustedIron_D, PBR);
		Plastic.Create(&Plastic_D, PBR);

		Importers::FontLoadingDesc fdesc;
		ArialFont = GetAssetManager().Import("@CommonAssets@/Fonts/arial.ttf", fdesc);
	}
	void SetupEntities()
	{
		//Create Entities
		auto EDirLight = GetScene().CreateEntity("DirLight");
		auto& dircomp = EDirLight.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Directional);
		dircomp.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));

		auto ELights = GetScene().CreateEntity("PointLight1");
		auto& lightcomp = ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		lightcomp.mCastShadows = true;

		auto ELights2 = GetScene().CreateEntity("PointLight2");
		auto& light2comp = ELights2.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		light2comp.mCastShadows = true;

		EController = GetScene().CreateEntity("Controller");

		//Assign Components
		EController.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Spot);
		EController.AddComponent<Components::CameraComponent>(&Camera);

		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(-2.0f, 4.0f, -1.0f));
	}


	void InitRenderer()
	{
		Renderer = GetScene().GetSystemManager().Add<Systems::RenderSystem>();

		Importers::ShaderLoadingDesc desc;
		desc.mType = Importers::ShaderType::_3DRendering;
		PBR = GetAssetManager().Import("@NuclearAssets@/Shaders/PBR/PBR.NEShader", desc);
		Renderer->RegisterShader(PBR);

		Renderer->AddRenderPass(&ShadowPass);
		Renderer->AddRenderPass(&GeoPass);
		//Renderer->AddRenderPass(&DefferedPass);
		Renderer->AddRenderPass(&PostFXPass);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);
		PostFXPass.Bake({ _Width_, _Height_,Renderer->mRenderData.mFinalRT.GetDesc() });

		mDebugSystem = GetScene().GetSystemManager().Add<Systems::DebugSystem>();
		mDebugSystem->Initialize(&Camera, Renderer->GetAnimationCB());

	}

	void Load()
	{
		GetAssetManager().Initialize();
		Rendering::ShadowPassBakingDesc spdesc;

		spdesc.MAX_OMNIDIR_CASTERS =2;
		spdesc.MAX_SPOT_CASTERS = 0;
		ShadowPass.Bake(spdesc);

		mCameraSystem = GetScene().GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		SetupEntities();

		InitRenderer();

		SetupAssets();


		// cubes
		{
			ECS::Transform TSphere;
			TSphere.SetPosition(Math::Vector3(0.0f, 1.5f, 0.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(2.0f, 0.0f, 1.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(-1.0f, 0.0f, 2.0));
			TSphere.SetRotation(glm::normalize(glm::vec3(1.0, 0.0, 1.0)), glm::radians(60.0f));
	//		TSphere.SetScale(Math::Vector3(0.25));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		GetScene().CreatePlane(&Plastic);

		Camera.RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);
		//Camera.MovementSpeed = 15;
		//Renderer->VisualizePointLightsPositions = true;
		//EController.GetComponent<Components::SpotLightComponent>().mCastShadows = true;

		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
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
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
	//	Renderer->ResizeRenderTargets(width, height);
	}
	void Update(float deltatime) override
	{
		//Movement
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_W))
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_A))
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_S))
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_D))
			Camera.ProcessMovement(Graphics::CAMERA_MOVEMENT_RIGHT, deltatime);

		//Change Mouse Mode
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_ESCAPE))
		{
			isMouseDisabled = true;
			Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Normal);
		}
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_M))
		{
			isMouseDisabled = false;
			Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
		}

		Camera.UpdateBuffer();
		mCameraSystem->Update(deltatime);
	}
	bool iskinematic = false;

	void Render(float dt) override
	{
		GetScene().Update(dt);
		static bool LockSpotlight = true;

		if (LockSpotlight)
		{
			EController.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Camera.GetPosition());
			EController.GetComponent<Components::LightComponent>().SetDirection(Camera.GetFrontView());
		}
		{
			using namespace Graphics;
			ImGui::Begin("PlayGround : Testing new features");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Image(ArialFont->mTextureView, {256.f,256.f });

			ImGui::ColorEdit3("Camera ClearColor", (float*)&Camera.RTClearColor);

			ImGui::Checkbox("ShowRegisteredRenderTargets", &mDebugSystem->ShowRegisteredRenderTargets);
			ImGui::Checkbox("LockSpotlight", &LockSpotlight);
			ImGui::Checkbox("RenderLightSources", &mDebugSystem->RenderLightSources);

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
				return Core::Engine::GetInstance().EndClient();
			}

			ImGui::End();
			EntityExplorer();
		}
	}

	void Shutdown() override
	{
		GetAssetManager().FlushContainers();
	}
};