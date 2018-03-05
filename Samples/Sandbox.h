#pragma once
#include "Common.h"

class Sandbox : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	//XAsset
	API::Texture DiffuseTex;
	API::Texture SpecularTex;
	API::Texture WhiteTex;

	//Default states
	API::CommonStates states;

	Components::FlyCamera Camera;

	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;

	//ECS
	Core::Scene SampleScene;

	Math::Vector3 lightPositions[4] = {
		Math::Vector3(-3.0f, 0.0f, 0.0f),
		Math::Vector3(-1.0f, 0.0f, 0.0f),
		Math::Vector3(1.0f, 0.0f, 0.0f),
		Math::Vector3(3.0f, 0.0f, 0.0f)
	};
	API::Color lightColors[4] = {
		API::Color(0.25),
		API::Color(0.50),
		API::Color(0.75),
		API::Color(1.00)
	};

	XAsset::ModelAsset Cube;
	XAsset::ModelAsset Lamp;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
public:
	Sandbox()
	{
	}

	void SetupLights()
	{
		pointlight1.SetPosition(lightPositions[0]);
		pointlight1.SetColor(lightColors[0]);

		pointlight2.SetPosition(lightPositions[1]);
		pointlight2.SetColor(lightColors[1]);

		pointlight3.SetPosition(lightPositions[2]);
		pointlight3.SetColor(lightColors[2]);

		pointlight4.SetPosition(lightPositions[3]);
		pointlight4.SetColor(lightColors[4]);
	}
	void SetupTextures()
	{
		API::Texture_Desc Desc;
		Desc.Format = API::Format::R8G8B8A8_UNORM_SRGB;
		Desc.Type = API::TextureType::Texture2D;
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/brickwall.jpg", &DiffuseTex, Desc);
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

		XAsset::ModelAsset::CreateCube(&Cube, textures);
		Cube.Initialize(&Renderer->GetVertexShader());
		
		XAsset::MeshTexture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.type = XAsset::MeshTextureType::Diffuse;
		std::vector<XAsset::MeshTexture> spheretextures;
		spheretextures.push_back(WhiteCTex);
		WhiteCTex.type = XAsset::MeshTextureType::Specular;
		spheretextures.push_back(WhiteCTex);

		XAsset::ModelAsset::CreateSphere(&Lamp, spheretextures);
		Lamp.Initialize(&Renderer->GetVertexShader());
		
	}

	void Load()
	{
		Systems::RenderSystemDesc desc;
		desc.GammaCorrection = true;
		desc.HDR = true;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer->InitializePostProcessing();
		Renderer->SetCamera(&Camera);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);

		Renderer->Bake();

		SetupLights();

		SetupXAsset();

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

	void mRenderscene(float dt)
	{
		states.EnabledDepth_DisabledStencil.Bind();
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);

		Math::Matrix4 CubeModelTrans;
		CubeModelTrans = Math::Rotate(CubeModelTrans, Math::Vector3(0.5f, 1.0f, 0.0f), ClockTime);
		Camera.SetModelMatrix(CubeModelTrans);
		Renderer->InstantRender(&Cube);

		for (unsigned int i = 0; i < 4; i++)
		{
			Math::Matrix4 model;
			model = Math::Translate(model, lightPositions[i]);
			model = Math::Scale(model, Math::Vector3(0.25f));
			Camera.SetModelMatrix(model);
			Renderer->InstantRender(&Lamp);
		}

		Renderer->Update_Light();

		SampleScene.Systems.Update_All(dt);
	}
	void Render(float dt) override
	{
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Tab))
		{
			Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			mRenderscene(dt);
			Core::Context::PresentFrame();
			return;
		}
		Renderer->RenderToPostProcessingRT();
		mRenderscene(dt);		

		states.DisabledDepthStencil.Bind();
		Renderer->RenderPostProcessingContents();

		SpecularTex.PSBind(0);
		Core::Context::PresentFrame();
	}
};