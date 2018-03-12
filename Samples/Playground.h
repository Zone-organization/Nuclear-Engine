#pragma once
#include "Common.h"

class Playground : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	//XAsset
	API::Texture DiffuseTex;
	API::Texture SpecularTex;
	API::Texture NormalTex;
	API::Texture WhiteTex;

	XAsset::ModelAsset SponzaAsset;


	XAsset::ModelAsset GridAsset;
	XAsset::ModelAsset SphereAsset;

	//Default states
	API::CommonStates states;

	Components::FlyCamera Camera;

	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::SpotLight spotLight;
	Components::DirectionalLight dirlight;

	//ECS
	Core::Scene SampleScene;


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
	Playground()
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
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/brickwall.jpg", &DiffuseTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/brickwall_normal.jpg", &NormalTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/black.png", &SpecularTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);



	}
	void SetupXAsset()
	{
		SetupTextures();

		std::vector<XAsset::MeshTexture> textures;

		XAsset::MeshTexture DTexture;
		DTexture.Texture = DiffuseTex;
		DTexture.type = XAsset::MeshTextureType::Diffuse;
		textures.push_back(DTexture);
		DTexture.Texture = SpecularTex;
		DTexture.type = XAsset::MeshTextureType::Specular;
		textures.push_back(DTexture);
		DTexture.Texture = NormalTex;
		DTexture.type = XAsset::MeshTextureType::Normal;
		textures.push_back(DTexture);

		XAsset::ModelAssetVertexDesc descm;
		descm.Tangents = true;
		XAsset::ModelAsset::CreateGrid(&GridAsset, textures, descm,2.0f,2.0f,2,2);
		GridAsset.Initialize(&Renderer->GetVertexShader());

		XAsset::MeshTexture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.type = XAsset::MeshTextureType::Diffuse;
		std::vector<XAsset::MeshTexture> spheretextures;
		spheretextures.push_back(WhiteCTex);
		WhiteCTex.type = XAsset::MeshTextureType::Specular;
		spheretextures.push_back(WhiteCTex);

		XAsset::ModelAsset::CreateSphere(&SphereAsset, spheretextures, descm);
		SphereAsset.Initialize(&Renderer->GetVertexShader());

		Managers::ModelLoadingDesc ModelDesc;
		ModelDesc.UseTangents = true;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;
		ModelDesc.LoadNormalTextures = true;
		Managers::AssetManager::LoadModel("Assets/Common/Models/CrytekSponza/sponza.obj", &SponzaAsset, ModelDesc);
		SponzaAsset.Initialize(&Renderer->GetVertexShader());

	}

	void Load()
	{
		Systems::RenderSystemDesc desc;
		desc.NormalMaps = true;
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

	void Render(float dt) override
	{


		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);
		states.DefaultSampler.PSBind(2);

		Math::Matrix4 NanosuitMatrix;
		NanosuitMatrix = Math::Translate(NanosuitMatrix, Math::Vector3(0.0f, -1.0f, 0.0f));
		NanosuitMatrix = Math::Scale(NanosuitMatrix, Math::Vector3(0.01f));
		Camera.SetModelMatrix(NanosuitMatrix);
		Renderer->InstantRender(&SponzaAsset);


		for (unsigned int i = 0; i < 4; i++)
		{
			Math::Matrix4 model;
			model = Math::Translate(model, pointLightPositions[i]);
			model = Math::Scale(model, Math::Vector3(0.25f));
			Camera.SetModelMatrix(model);
			Renderer->InstantRender(&SphereAsset);
		}

		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		Renderer->Update_Light();

		SampleScene.Systems.Update_All(dt);

		states.EnabledDepth_DisabledStencil.Bind();

		Core::Context::PresentFrame();
	}
};