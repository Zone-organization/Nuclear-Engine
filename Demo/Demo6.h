#pragma once
#include "Common.h"

class Demo6 : public Core::Game
{
protected:
	API::Shader *Shader;

	Components::FlyCamera *Camera;

	Components::Model *model;

	// positions all containers
	Math::Vector3 cubePositions[10] = {
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
	Math::Vector3 pointLightPositions[4] = {
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f)
	};

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Demo6()
	{
	}
	void Load()
	{
		if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
		{
			Shader = new API::Shader("ModelShader",
				Core::FileSystem::LoadFileToString("Assets/Demo6/Shaders/OpenGL/model.vs").c_str(),
				Core::FileSystem::LoadFileToString("Assets/Demo6/Shaders/OpenGL/model.fs").c_str(),
				nullptr,
				ShaderLanguage::GLSL);
		}
		else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
		{
			Shader = new API::Shader("ModelShader",
				Core::FileSystem::LoadFileToString("Assets/Demo6/Shaders/DirectX/model.vs").c_str(),
				Core::FileSystem::LoadFileToString("Assets/Demo6/Shaders/DirectX/model.ps").c_str(),
				nullptr,
				ShaderLanguage::HLSL);
		}
	
		Camera = new Components::FlyCamera();
		Camera->Initialize(Math::Perspective(Math::Deg2Rad(45.0f), (float)_Width_ / (float)_Height_, 0.1f, 100.0f));
		Shader->SetUniformBuffer(Camera->GetCBuffer());

		model = new Components::Model("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", Shader);

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

		//Input::Mouse::SetInputMode(Input::Mouse::InputMode::Virtual);
		//Input::Mouse::ShowMouseCursor(false);
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
		Core::Context::ClearColor(API::Color(0.1f, 0.1f, 0.1f, 1.0f));
		Shader->Bind();

		model->Draw();
		Shader->Unbind();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{

	}
};