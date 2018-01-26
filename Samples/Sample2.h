#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
protected:
	API::Texture DiffuseTex;
	API::Texture SpecularTex;

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
	Components::SpotLight spotLight;
	Components::Model Nanosuit;

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
	Math::Vector3 pointLightPositions[6] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f),
		Math::Vector3(4.0f,  3.0f, -2.0f),
		Math::Vector3(6.0f, 0.0f, 0.0f)
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
		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatio(), 0.1f, 100.0f));

		Renderer.SetCamera(&Camera);
		Renderer.AddLight(&spotLight);
		Renderer.AddLight(&pointlight1);
		Renderer.AddLight(&pointlight2);
		Renderer.AddLight(&pointlight3);
		Renderer.AddLight(&pointlight4);
		Renderer.AddLight(&pointlight5);
		Renderer.AddLight(&pointlight6);

		Renderer.AddLight(&dirlight);
		Renderer.SetTechnique(&LightShading);
		Renderer.Bake();

		dirlight.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dirlight.SetColor(API::Color(0.4f, 0.4f, 0.4f, 0.0f));

		// point light 1
		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(API::Color(1.0f, 1.0f, 1.0f, 0.0f));

		//point light 2
		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		// point light 3
		pointlight3.SetPosition(pointLightPositions[2]);
		pointlight3.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		// point light 4
		pointlight4.SetPosition(pointLightPositions[3]);
		pointlight4.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));
		

		pointlight5.SetPosition(pointLightPositions[4]);
		pointlight5.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight6.SetPosition(pointLightPositions[5]);
		pointlight6.SetColor(API::Color(0.8f, 0.8f, 0.8f, 0.0f));

		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &DiffuseTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_specular.png", &SpecularTex, Desc);

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
		AssetManager::LoadModel("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", &Nanosuit);

		Core::Context::EnableDepthBuffer(true);
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);

		Core::Application::Display();
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

	void OnMouseMovement(float xpos, float ypos) override
	{
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
	void Render(float) override
	{
		Core::Context::Begin();

		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			Math::Matrix4 model;
			model = Math::Translate(model, cubePositions[i]);
			//float angle = 20.0f * i;
			//model = Math::Rotate(model, Math::Vector3(1.0f, 0.3f, 0.5f), Math::ToRadians(angle));
			Camera.SetModelMatrix(model);

			Cube.Draw(&Renderer.GetShader());
		}


		Math::Matrix4 NanosuitMatrix;
		NanosuitMatrix = Math::Translate(NanosuitMatrix, Math::Vector3(5.0f, -1.75f, 0.0f));
		NanosuitMatrix = Math::Scale(NanosuitMatrix, Math::Vector3(0.25f, 0.25f, 0.25f));
		Camera.SetModelMatrix(NanosuitMatrix);
		Nanosuit.Draw(&Renderer.GetShader());
		
		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::U))
		{
			pointlight6.SetIntensity(2.0f);
		}
	
		Renderer.Render_Light();

		Core::Context::End();
	}
};