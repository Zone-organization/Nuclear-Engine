#pragma once
#include "Common.h"

class Demo5 : public Core::Game
{
protected:
	API::Texture DiffuseTex;
	API::Texture SpecularTex;

	Components::FlyCamera Camera;
	Shading::Techniques::NoLight *LightShading;
	Components::Cube *Cube;
	Components::Cube *Lamp;

	Components::Light dirlight;
	Components::Light pointlight1;
	Components::Light pointlight2;
	Components::Light pointlight3;
	Components::Light pointlight4;

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
	Math::Vector3 pointLightPositions[4] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f)
	};

	Renderers::Renderer3D* Renderer;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Demo5()
		: pointlight1(Components::Light::Type::PointLight),
		pointlight2(Components::Light::Type::PointLight),
		pointlight3(Components::Light::Type::PointLight),
		pointlight4(Components::Light::Type::PointLight),
		dirlight(Components::Light::Type::DirectionalLight)
	{
	}
	void Load()
	{
		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), (float)800 / (float)600, 0.1f, 100.0f));

		Renderer = new Renderers::Renderer3D(Camera);

		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();

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


		Texture_Desc Desc;
		Desc.Filter = TextureFilter::Trilinear;
		Desc.Wrap = TextureWrap::Repeat;
		Desc.Format = TextureFormat::R8G8B8A8;
		Desc.Type = TextureType::Texture2D;

		DiffuseTex = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/crate_diffuse.png", Desc), Desc);
		SpecularTex = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/crate_diffuse.png", Desc), Desc);

		Shading::Material CubeMat;
		CubeMat.Diffuse = DiffuseTex;
		CubeMat.Specular = SpecularTex;
		Cube = new Components::Cube(Components::InputSignatures::Position_Normal_Texcoord, &CubeMat);
		//Lamp = new Components::Cube(Components::InputSignatures::Position, &CubeMat);

		Core::Context::EnableDepthBuffer(true);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void PreRender(float deltatime) override
	{
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::W))
			Camera->ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::A))
			Camera->ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::S))
			Camera->ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::D))
			Camera->ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera->Update();
	}

	void MouseMovementCallback(double xpos, double ypos) override
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		Camera->ProcessEye(xoffset, yoffset);
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearFlags::Depth);

		Renderer->GetShader()->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			Math::Matrix4 model;
			model = Math::Translate(model, cubePositions[i]);
			//float angle = 20.0f * i;
			//model = Math::Rotate(model, Math::Vector3(1.0f, 0.3f, 0.5f), Math::ToRadians(angle));
			Camera->SetModelMatrix(model);

			Cube->Draw(Renderer->GetShader());
		}
		
		Renderer->GetShader()->Unbind();

		Renderer->Render_Light();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
//		delete LightShading;
		delete Cube;
		delete Camera;
	}
};