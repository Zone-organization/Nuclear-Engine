#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	bool renderspheres = true;
	bool rendersponza = true;
	bool renderimgui = true;

	bool usenormalmaps = false;

	//Assets
	Graphics::API::Texture WhiteTex;

	Assets::Mesh SphereAsset;
	Assets::Mesh SponzaAsset;

	//Default states
	Graphics::API::CommonStates states;

	Components::CameraComponent Camera;

	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::SpotLight spotLight;

	//ECS
	ECS::Scene SampleScene;
	ECS::Entity ELamp;
	ECS::Entity ENanosuit;

	Components::MeshComponent LampModel;
	Components::MeshComponent SponzaModel;

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
		dirlight.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));

		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight3.SetPosition(pointLightPositions[2]);
		pointlight3.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight4.SetPosition(pointLightPositions[3]);
		pointlight4.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

	}
	void SetupTextures()
	{
		Graphics::API::Texture_Desc Desc;
		Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
		Desc.Type = Graphics::API::TextureType::Texture2D;

		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);
	}
	void SetupAssets()
	{
		SetupTextures();

		Assets::MeshTexture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.type = Assets::MeshTextureType::Diffuse;
		std::vector<Assets::MeshTexture> spheretextures;
		spheretextures.push_back(WhiteCTex);
		WhiteCTex.type = Assets::MeshTextureType::Specular;
		spheretextures.push_back(WhiteCTex);
		
		Assets::MeshVertexDesc descsphere;
		if (usenormalmaps)
		{
			WhiteCTex.type = Assets::MeshTextureType::Normal;
			spheretextures.push_back(WhiteCTex);

			descsphere.Tangents = true;
		}
		Assets::Mesh::CreateSphere(&SphereAsset, spheretextures);
		SphereAsset.Initialize(&Renderer->GetVertexShader());

		Managers::MeshLoadingDesc ModelDesc;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;

		if (usenormalmaps)
		{
			ModelDesc.UseTangents = true;
			ModelDesc.LoadNormalTextures = true;
		}
		Managers::AssetManager::LoadModel("Assets/Common/Models/CrytekSponza/sponza.obj", &SponzaAsset, ModelDesc);
		SponzaAsset.Initialize(&Renderer->GetVertexShader());

	}
	void SetupEntities()
	{
		LampModel.mMesh = &SphereAsset;
		SponzaModel.mMesh = &SponzaAsset;
	}
	void Load()
	{
		std::cout << "Use Normal Maps (1 = True, else = False): ";
		char i;
		std::cin >> i;

		if (i == '1')
		{
			usenormalmaps = true;
		}
		else
		{
			usenormalmaps = false;
		}

		Systems::RenderSystemDesc desc;
		if (usenormalmaps)
			desc.NormalMaps = true;

		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		Camera.Initialize(Math::Perspective(Math::radians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer->SetCamera(&Camera);
		Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();

		SetupLights();

		SetupAssets();

		SetupEntities();


		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 16.0f);

		SampleScene.Entities;

		ImGui::StyleColorsDark();

		states.EnabledDepth_DisabledStencil.Bind();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);

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

		if (Core::Input::Keyboard::IsKeyPressed(Core::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Input::Keyboard::IsKeyPressed(Core::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Input::Keyboard::IsKeyPressed(Core::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Input::Keyboard::IsKeyPressed(Core::Input::Keyboard::Key::D))
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
		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);

		if (renderspheres)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				Math::Matrix4 model(1.0f);
				model = Math::Translate(model, pointLightPositions[i]);
				model = Math::Scale(model, Math::Vector3(0.25f));
				Camera.SetModelMatrix(model);
				Renderer->InstantRender(&LampModel);
			}
		}
		if (rendersponza)
		{
			Math::Matrix4 SponzaMatrix(1.0f);
			SponzaMatrix = Math::Translate(SponzaMatrix, Math::Vector3(0.0f, -1.0f, 0.0f));
			SponzaMatrix = Math::Scale(SponzaMatrix, Math::Vector3(0.01f));
			Camera.SetModelMatrix(SponzaMatrix);
			Renderer->InstantRender(&SponzaModel);
		}
		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		Renderer->Update_Light();

		SampleScene.Systems.Update_All(dt);

		states.EnabledDepth_DisabledStencil.Bind();

		Graphics::API::Context::PresentFrame();
	}
};