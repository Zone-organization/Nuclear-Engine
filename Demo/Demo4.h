#pragma once
#include "Common.h"

class Demo4 : public Core::Game
{
protected:
	API::Shader CubeShader;
	API::Texture WoodenBoxTex;
	Components::FlyCamera Camera;

	Components::Cube *Cube;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Demo4()
	{
	}
	void Load()
	{
		API::Shader::Create(&CubeShader,
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/CubeShader.vs").c_str(),API::ShaderType::Vertex,API::ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/CubeShader.ps").c_str(),API::ShaderType::Pixel,API::ShaderLanguage::HLSL));

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), (float)800 / (float)600, 0.1f, 100.0f));
		CubeShader.SetConstantBuffer(&Camera.GetCBuffer() ,API::ShaderType::Vertex);

		Texture_Desc Desc;
		Desc.Filter = TextureFilter::Trilinear;
		Desc.Wrap = TextureWrap::Repeat;
		Desc.Format = TextureFormat::R8G8B8A8;
		Desc.Type = TextureType::Texture2D;

		API::Texture::Create(&WoodenBoxTex, &ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", Desc), &Desc);

		Shading::Material CubeMat;
		CubeMat.Diffuse = &WoodenBoxTex;
		Cube = new Components::Cube(Components::InputSignatures::Position_Texcoord, &CubeMat);

		Core::Context::EnableDepthBuffer(true);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void PreRender(float deltatime) override
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

		Camera.ProcessEye(xoffset, yoffset);
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearFlags::Depth);

		Cube->Draw(&CubeShader);

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
		delete Cube;
	}
};