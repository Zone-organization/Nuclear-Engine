#pragma once
#include "Common.h"

class Sample1 : public Core::Game
{
protected:
	API::Texture WoodenBoxTex;
	Components::FlyCamera Camera;
	API::Shader CubeShader;

	Components::Model Nanosuit;
	Components::Model Cube;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Sample1()
	{
	}
	void Load()
	{
		API::ShaderDesc desc;
		desc.Name = "Sample1";
		API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/CubeShader.vs").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/CubeShader.ps").c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

		API::Shader::Create(&CubeShader, &desc);
	
		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatio(), 0.1f, 100.0f));
		CubeShader.SetConstantBuffer(&Camera.GetCBuffer() ,API::ShaderType::Vertex);

		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, Desc);

		Shading::Material CubeMat;
		CubeMat.Diffuse = WoodenBoxTex;
		Components::Model::CreateCube(&Cube, &CubeMat);
		
		AssetManager::LoadModel("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", &Nanosuit);

		Core::Application::Display();

		Core::Context::EnableDepthBuffer(true);
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
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
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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
	void Render(float deltatime) override
	{
		Core::Context::Begin();
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		Math::Matrix4 CubeMatrix;
		CubeMatrix = Math::Translate(CubeMatrix, Math::Vector3(3.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		Camera.SetModelMatrix(CubeMatrix);
		Cube.Draw(&CubeShader);

		Math::Matrix4 NanosuitMatrix;
		NanosuitMatrix = Math::Translate(NanosuitMatrix, Math::Vector3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		NanosuitMatrix = Math::Scale(NanosuitMatrix, Math::Vector3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		Camera.SetModelMatrix(NanosuitMatrix);
		Nanosuit.Draw(&CubeShader);

		Core::Context::End();
	}
	void Shutdown() override 
	{
		API::Texture::Delete(&WoodenBoxTex);
		API::Shader::Delete(&CubeShader);
	}
};