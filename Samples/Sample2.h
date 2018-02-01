#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
protected:
	API::Texture DiffuseTex;
	API::Texture SpecularTex;
	API::Texture WhiteTex;

	Components::FlyCamera Camera;
	Shading::Techniques::PhongShading LightShading;
	Components::Model Cube;
	Components::Model Lamp;

	Renderers::Renderer3D Renderer;
	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::PointLight pointlight5;
	Components::PointLight pointlight6;
	Components::PointLight pointlight7;
	Components::PointLight pointlight8;
	Components::PointLight pointlight9;

	Components::SpotLight spotLight;
	Components::Model Nanosuit;

	Components::Skybox Skybox;

	// positions all containers
	Math::Vector3 cubePositions[10] = 
	{
		Math::Vector3(0.0f,  0.0f,  0.0f),
		Math::Vector3(2.0f,  5.0f, -15.0f),
		Math::Vector3(-1.5f, -2.2f, -2.5f),
		Math::Vector3(-3.8f, -2.0f, -12.3f),
		Math::Vector3(2.4f, -0.4f, -3.5f),
		Math::Vector3(-1.7f,  3.0f, -7.5f),
		Math::Vector3(1.3f, -2.0f, -2.5f),
		Math::Vector3(1.5f,  2.0f, -2.5f),
		Math::Vector3(1.5f,  0.2f, -1.5f),
		Math::Vector3(-1.3f,  1.0f, -1.5f)
	};

	// positions of the point lights
	Math::Vector3 pointLightPositions[9] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f),
		Math::Vector3(4.0f,  3.0f, -2.0f),
		Math::Vector3(6.2f, 2.0f, 0.0f),
		Math::Vector3(6.2f, -2.0f, 0.0f),
		Math::Vector3(-6.2f, 2.0f, 0.0f),
		Math::Vector3(-6.2f, -2.0f, 0.0f)
	};
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
public:
	Sample2()
	{
	}
	void Load()
	{
		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer.SetCamera(&Camera);
		Renderer.AddLight(&spotLight);
		Renderer.AddLight(&pointlight1);
		Renderer.AddLight(&pointlight2);
		Renderer.AddLight(&pointlight3);
		Renderer.AddLight(&pointlight4);
		Renderer.AddLight(&pointlight5);
		Renderer.AddLight(&pointlight6);
		Renderer.AddLight(&pointlight7);
		Renderer.AddLight(&pointlight8);
		Renderer.AddLight(&pointlight9);

		Renderer.AddLight(&dirlight);
		Renderer.SetTechnique(&LightShading);
		Renderer.Bake();

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
		
		pointlight5.SetPosition(pointLightPositions[4]);
		pointlight5.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight6.SetPosition(pointLightPositions[5]);
		pointlight6.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight7.SetPosition(pointLightPositions[6]);
		pointlight7.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight8.SetPosition(pointLightPositions[7]);
		pointlight8.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight9.SetPosition(pointLightPositions[8]);
		pointlight9.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &DiffuseTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_specular.png", &SpecularTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);

		std::vector<Components::MeshTexture> Textures;
		Components::MeshTexture DiffuseCTex;
		DiffuseCTex.Texture = DiffuseTex;
		DiffuseCTex.type = Components::MeshTextureType::Diffuse;

		Components::MeshTexture SpecularCTex;
		SpecularCTex.Texture = SpecularTex;
		SpecularCTex.type = Components::MeshTextureType::Specular;

		Textures.push_back(DiffuseCTex);
		Textures.push_back(SpecularCTex);

		Components::Model::CreateCube(&Cube, Textures);

		Components::MeshTexture WhiteCTex;
		WhiteCTex.Texture = WhiteTex;
		WhiteCTex.type = Components::MeshTextureType::Specular;
		Components::Model::CreateSphere(&Lamp, std::vector<Components::MeshTexture>{WhiteCTex});

		ModelLoadingDesc ModelDesc;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;
		AssetManager::LoadModel("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", &Nanosuit, ModelDesc);

		//Create The skybox
		std::array<std::string, 6> SkyBoxTexturePaths
		{
			std::string("Assets/Common/Skybox/right.jpg"),
			std::string("Assets/Common/Skybox/left.jpg"),
			std::string("Assets/Common/Skybox/top.jpg"),
			std::string("Assets/Common/Skybox/bottom.jpg"),
			std::string("Assets/Common/Skybox/front.jpg"),
			std::string("Assets/Common/Skybox/back.jpg")
		};

		Skybox.Create(&Camera, SkyBoxTexturePaths);


		Core::Context::EnableDepthBuffer(true);
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

	void Render(float) override
	{
		Core::Context::Begin();

		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			Math::Matrix4 model;
			model = Math::Translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = Math::Rotate(model, Math::Vector3(1.0f, 0.3f, 0.5f), Math::ToRadians(angle));
			Camera.SetModelMatrix(model);

			Cube.Draw(&Renderer.GetShader());
		}
		for (unsigned int i = 0; i < 9; i++)
		{
			Math::Matrix4 model;
			model = Math::Translate(model, pointLightPositions[i]);
			model = Math::Scale(model, Math::Vector3(0.25f));
			Camera.SetModelMatrix(model);
			Lamp.Draw(&Renderer.GetShader());
		}

		Math::Matrix4 NanosuitMatrix;
		NanosuitMatrix = Math::Translate(NanosuitMatrix, Math::Vector3(5.0f, -1.75f, 0.0f));
		NanosuitMatrix = Math::Scale(NanosuitMatrix, Math::Vector3(0.25f));
		Camera.SetModelMatrix(NanosuitMatrix);
		Nanosuit.Draw(&Renderer.GetShader());
		
		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());
	
		Renderer.Render_Light();

		Skybox.Render();

		Core::Context::End();
	}
};