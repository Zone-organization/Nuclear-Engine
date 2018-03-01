#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	bool renderspheres = true;
	bool rendersponza = true;
	bool renderimgui = true;

	//XAsset
	API::Texture WhiteTex;

	XAsset::ModelAsset SphereAsset;
	XAsset::ModelAsset SponzaAsset;

	//Default states
	API::CommonStates states;

	Components::FlyCamera Camera;

	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::SpotLight spotLight;

	//ECS
	Core::Scene SampleScene;
	Core::Entity ELamp;
	Core::Entity ENanosuit;

	Components::Model LampModel;
	Components::Model SponzaModel;

	// positions of the point lights
	Math::Vector3 pointLightPositions[4] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f)
	};
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
public:
	Sample2()
	{
	}

	void SetupLights()
	{
		dirlight.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dirlight.SetColor(API::Color(0.4f, 0.4f, 0.4f, 0.0f));

		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(API::Color(1.0f, 1.0f, 1.0f, 0.0f));

		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight3.SetPosition(pointLightPositions[2]);
		pointlight3.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight4.SetPosition(pointLightPositions[3]);
		pointlight4.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

	}
	void SetupTextures()
	{
		API::Texture_Desc Desc;
		Desc.Format = API::Format::R8G8B8A8_UNORM;
		Desc.Type = API::TextureType::Texture2D;

		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);
	}
	void SetupXAsset()
	{
		SetupTextures();

		XAsset::MeshTexture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.type = XAsset::MeshTextureType::Diffuse;
		std::vector<XAsset::MeshTexture> spheretextures;
		spheretextures.push_back(WhiteCTex);
		WhiteCTex.type = XAsset::MeshTextureType::Specular;
		spheretextures.push_back(WhiteCTex);

		XAsset::ModelAsset::CreateSphere(&SphereAsset, spheretextures);
		SphereAsset.Initialize(&Renderer->GetVertexShader());

		Managers::ModelLoadingDesc ModelDesc;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;
		Managers::AssetManager::LoadModel("Assets/Common/Models/CrytekSponza/sponza.obj", &SponzaAsset, ModelDesc);
		SponzaAsset.Initialize(&Renderer->GetVertexShader());

	}
	void SetupEntities()
	{
		LampModel.SetAsset(&SphereAsset);
		//ELamp = SampleScene.Entities.Create();
		//ELamp.Assign<Components::Model>(LampModel);
		SponzaModel.SetAsset(&SponzaAsset);
		//ENanosuit = SampleScene.Entities.Create();
		//ENanosuit.Assign<Components::Model>(SponzaModel);
	}
	void Load()
	{
		Systems::RenderSystemDesc desc;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer->SetCamera(&Camera);
		Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();

		SetupLights();

		SetupXAsset();

		SetupEntities();


		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 16.0f);

		SampleScene.Entities;

		ImGui::StyleColorsDark();

		states.EnabledDepth_DisabledStencil.Bind();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void OnMouseMovement(double xpos_a, double ypos_a) override
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

	void Update(float deltatime) override
	{
		//Core::Application::SetTitle("Nuclear Engine FPS: " + std::to_string(FPS) + " FrameTime: " + std::to_string(FrameTime));

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::D))
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera.Update();
	}
	void ShowOverlay(bool show)
	{
		const float DISTANCE = 10.0f;
		static int corner = 0;
		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
		if (ImGui::Begin("FPS Overlay", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Text("FPS Overlay\n");
			ImGui::Separator();
			ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		ImGui::PopStyleColor();
	}
	void Render(float dt) override
	{
		Core::Context::Begin();
		//ImGui::NE_NewFrame();

		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);

		if (renderspheres)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				Math::Matrix4 model;
				model = Math::Translate(model, pointLightPositions[i]);
				model = Math::Scale(model, Math::Vector3(0.25f));
				Camera.SetModelMatrix(model);
				Renderer->InstantRender(&LampModel);
			}
		}
		if (rendersponza)
		{
			Math::Matrix4 NanosuitMatrix;
			NanosuitMatrix = Math::Translate(NanosuitMatrix, Math::Vector3(0.0f, -1.0f, 0.0f));
			NanosuitMatrix = Math::Scale(NanosuitMatrix, Math::Vector3(0.01f));
			Camera.SetModelMatrix(NanosuitMatrix);
			Renderer->InstantRender(&SponzaModel);
		}
		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		Renderer->Update_Light();

		SampleScene.Systems.Update_All(dt);

		states.EnabledDepth_DisabledStencil.Bind();

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Tab))
		{
			if (renderimgui == true)
			{
			//	renderimgui = false;
			//	Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
			}
			else
			{
			//	renderimgui = true;
			//	Core::Application::SetMouseInputMode(Core::MouseInputMode::Normal);
			}
		}

		if (renderimgui == true)
		{
		//	ImGui::Checkbox("Render Lamps Spheres", &renderspheres);
		//	ImGui::Checkbox("Render Sponza", &rendersponza);
		}
		//ShowOverlay(true);

		//ImGui::Render();
		Core::Context::End();
	}
};