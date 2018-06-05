#pragma once
#include "Common.h"

class Sandbox : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	//Audio::Sound sound;
	//Audio::Channel channel;

	//Assets
	Assets::Texture DiffuseTex;
	Assets::Texture SpecularTex;
	Assets::Texture WhiteTex;

	//Default states
	Graphics::API::CommonStates states;

	Components::CameraComponent Camera;

	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;

	//ECS
	ECS::Scene SampleScene;
	ECS::Entity ECube;
	ECS::Entity ELamp;

	Math::Vector3 lightPositions[4] = {
		Math::Vector3(-3.0f, 0.0f, 0.0f),
		Math::Vector3(-1.0f, 0.0f, 0.0f),
		Math::Vector3(1.0f, 0.0f, 0.0f),
		Math::Vector3(3.0f, 0.0f, 0.0f)
	};
	Graphics::Color lightColors[4] = {
		Graphics::Color(0.25),
		Graphics::Color(0.50),
		Graphics::Color(0.75),
		Graphics::Color(1.00)
	};

	Assets::Mesh Cube;
	Assets::Mesh Lamp;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
public:
	Sandbox()
	{
	}
/*
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
		Assets::Texture_Desc Desc;
		Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM_SRGB;
		Desc.Type = Assets::TextureUsageType::Texture2D;
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/brickwall.jpg", &DiffuseTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/black.png", &SpecularTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);
	}
	void SetupAssets()
	{
		SetupTextures();

		std::vector<Assets::Texture> textures;

		Assets::Texture DTexture;
		DTexture.Texture = DiffuseTex;
		DTexture.SetUsageType(Assets::TextureUsageType::Diffuse;
		textures.push_back(DTexture);
		DTexture.Texture = SpecularTex;
		DTexture.SetUsageType(Assets::TextureUsageType::Specular);
		textures.push_back(DTexture);

		Assets::Mesh::CreateCube(&Cube, textures);
		Cube.Initialize(&Renderer->GetVertexShader());
		
		Assets::Texture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.SetUsageType(Assets::TextureUsageType::Diffuse;
		std::vector<Assets::Texture> spheretextures;
		spheretextures.push_back(WhiteCTex);
		WhiteCTex.SetUsageType(Assets::TextureUsageType::Specular_;
		spheretextures.push_back(WhiteCTex);

		Assets::Mesh::CreateSphere(&Lamp, spheretextures);
		Lamp.Initialize(&Renderer->GetVertexShader());
		
	}
	void SetupRenderer()
	{
		Systems::RenderSystemDesc desc;
		desc.GammaCorrection = true;
		desc.HDR = true;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		Renderer->InitializePostProcessing();
		Renderer->SetCamera(&Camera);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);

		Renderer->Bake();
	}
	void Load()
	{
	
		//sound.Create("Assets/Common/Sounds/yurimaster.wav",SOUND_MODE_DEFAULT);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));


		SetupLights();

		SetupAssets();

		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void OnMouseMovement(int xpos_a, int ypos_a) override
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
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::D))
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

		Math::Matrix4 CubeModelTrans(1.0f);
		CubeModelTrans = Math::rotate(CubeModelTrans, ClockTime, Math::Vector3(0.5f, 1.0f, 0.0f));
		Camera.SetModelMatrix(CubeModelTrans);
		Renderer->InstantRender(&Cube);

		for (unsigned int i = 0; i < 4; i++)
		{
			Math::Matrix4 model(1.0f);
			model = Math::translate(model, lightPositions[i]);
			model = Math::scale(model, Math::Vector3(0.25f));
			Camera.SetModelMatrix(model);
			Renderer->InstantRender(&Lamp);
		}

		Renderer->Update_Light();

		SampleScene.Systems.Update_All(dt);
	}
	void Render(float dt) override
	{
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Tab))
		{
			Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			mRenderscene(dt);
			Graphics::API::Context::PresentFrame();
			return;
		}
		Renderer->RenderToPostProcessingRT();
		mRenderscene(dt);

		states.DisabledDepthStencil.Bind();
		Renderer->RenderPostProcessingContents();

		SpecularTex.PSBind(0);

		//sound.Play(&channel);
		
		//Audio::AudioEngine::Update(&channel);
		Graphics::API::Context::PresentFrame();
	}*/
};