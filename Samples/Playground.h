#pragma once
#include "Common.h"

class Playground : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	//XAsset
	Graphics::API::Texture DiffuseTex;
	Graphics::API::Texture SpecularTex;
	Graphics::API::Texture NormalTex;
	Graphics::API::Texture WhiteTex;

	XAsset::ModelAsset GridAsset;
	XAsset::ModelAsset SphereAsset;

	//Default states
	Graphics::API::CommonStates states;

	Components::FlyCamera Camera;

	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::SpotLight spotLight;

	//ECS
	Core::Scene SampleScene;

	// positions of the point lights
	Math::Vector3 pointLightPositions[2] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(0.5f, 1.0f, 0.3f)
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
		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
	}
	void SetupTextures()
	{
		Graphics::API::Texture_Desc Desc;
		Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
		Desc.Type = Graphics::API::TextureType::Texture2D;
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
		XAsset::ModelAsset::CreateGrid(&GridAsset, textures, descm,2.0f, 2.0f, 5, 5);
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
	}

	void Load()
	{
		Systems::RenderSystemDesc desc;
		desc.NormalMaps = true;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		Camera.Initialize(Math::Perspective(Math::radians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer->SetCamera(&Camera);
		//Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->Bake();

		SetupLights();

		SetupXAsset();

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


		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);
		states.DefaultSampler.PSBind(2);

		Math::Matrix4 CubeModelTrans(1.0f);
		CubeModelTrans = Math::Rotate(CubeModelTrans, Math::radians(ClockTime * -10.0f), Math::Vector3(1.0f, 0.0f, 1.0f));
		Camera.SetModelMatrix(CubeModelTrans);
		Renderer->InstantRender(&GridAsset);

		for (unsigned int i = 0; i < 2; i++)
		{
			Math::Matrix4 model(1.0f);
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

		Graphics::API::Context::PresentFrame();
	}
};