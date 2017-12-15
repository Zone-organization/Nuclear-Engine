#pragma once
#include "Common.h"

class Demo5 : public Core::Game
{
protected:
	API::Texture *WoodenBoxTex;
	Components::FlyCamera *Camera;
	Shading::Techniques::NoLight *LightShading;
	Components::Cube *Cube;

	Components::Light *DirLight;
	Components::Light PointLight1;

	Renderers::Renderer3D* Renderer;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Demo5()
		: PointLight1(Components::Light::Type::PointLight)
	{
	}
	void Load()
	{
		Camera = new Components::FlyCamera();
		Camera->Initialize(Math::Perspective(Math::ToRadians(45.0f), (float)800 / (float)600, 0.1f, 100.0f));

		Renderer = new Renderers::Renderer3D(Camera);

		// directional light
		DirLight = new Components::Light(Components::Light::Type::DirectionalLight);
		DirLight->SetDirection((-0.2f, -1.0f, -0.3f));
		DirLight->SetColor((0.5f, 0.5f, 0.5f));

		PointLight1.SetPosition((1.2f, 1.0f, 2.0f));
		PointLight1.SetColor((1.0f, 1.0f, 1.0f));
		Renderer->AddLight(&PointLight1);
		Renderer->Bake();

		Texture_Desc Desc;
		Desc.Filter = TextureFilter::Trilinear;
		Desc.Wrap = TextureWrap::Repeat;
		Desc.Format = TextureFormat::R8G8B8A8;
		Desc.Type = TextureType::Texture2D;

		WoodenBoxTex = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/crate_diffuse.png", Desc), Desc);

		Shading::Material CubeMat;
		CubeMat.Diffuse = WoodenBoxTex;
		Cube = new Components::Cube(Components::InputSignatures::Position_Normal_Texcoord, &CubeMat);
		
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
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));
		//Don't Forget to clear the depth buffer each frame
		Core::Context::ClearDepthBuffer();

		Renderer->GetShader()->Bind();

		Cube->Draw(Renderer->GetShader());

		Renderer->GetShader()->Unbind();

		Renderer->Render_Light();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
		delete LightShading;
		delete Cube;
		delete Camera;
		delete WoodenBoxTex;
	}
};