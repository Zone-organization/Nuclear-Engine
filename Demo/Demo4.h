#pragma once
#include "Common.h"

class Demo4 : public Core::Game
{
protected:
	API::Shader *CubeShader;
	//API::VertexBuffer *CubeVB;
	//API::InputLayout *CubeIL;
	API::Texture2D *WoodenBoxTex;
	Components::FlyCamera *Camera;

	Components::Cube *cube;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Demo4()
	{
	}
	void Load()
	{
		CubeShader = new API::Shader("CubeShader",
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/DirectX/CubeShader.vs").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/DirectX/CubeShader.ps").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));

		Camera = new Components::FlyCamera();
		Camera->Initialize(Math::Perspective(Math::Deg2Rad(45.0f), (float)800 / (float)600, 0.1f, 100.0f));
		CubeShader->SetUniformBuffer(Camera->GetCBuffer() ,0, ShaderType::Vertex);

		Texture_Desc Desc;
		Desc.Filter = TextureFilter::Trilinear;
		Desc.Wrap = TextureWrap::Repeat;
		Desc.Format = TextureFormat::R8G8B8A8;

		WoodenBoxTex = new API::Texture2D(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", Desc), Desc);
		cube = new Components::Cube(WoodenBoxTex);

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
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::Z))
		{

		}
		else {
			Camera->Update();
		}
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

		//WoodenBoxTex->Bind(0);
		//CubeShader->Bind();
		//CubeVB->Bind();
		//Core::Context::Draw(36);
		//CubeVB->Unbind();
		CubeShader->Bind();
		cube->Draw(CubeShader);
		//CubeShader->Unbind();
		//WoodenBoxTex->Unbind();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
		delete CubeShader;
		//delete CubeVB;
		delete WoodenBoxTex;
	}
};


/*
#pragma once
#include "Common.h"

class Demo4 : public Core::Game
{
protected:
API::Shader *CubeShader;
API::VertexBuffer *CubeVB;
API::InputLayout *CubeIL;
API::Texture2D *WoodenBoxTex;
Components::FlyCamera *Camera;

float lastX = _Width_ / 2.0f;
float lastY = _Height_ / 2.0f;
bool firstMouse = true;

public:
Demo4()
{
}
void Load()
{
if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
{
CubeShader = new API::Shader("CubeShader",
Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/OpenGL/CubeShader.vs").c_str(),
Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/OpenGL/CubeShader.fs").c_str(),
nullptr,
ShaderLanguage::GLSL);
}
else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
{
CubeShader = new API::Shader("CubeShader",
Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/DirectX/CubeShader.vs").c_str(),
Core::FileSystem::LoadFileToString("Assets/Demo4/Shaders/DirectX/CubeShader.ps").c_str(),
nullptr,
ShaderLanguage::HLSL);
}

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

API::VertexBufferDesc vDesc;
vDesc.data = vertices;
vDesc.size = sizeof(vertices);
vDesc.usage = BufferGPUUsage::Dynamic;
vDesc.accessflag = BufferCPUAccess::Default;

CubeVB = new API::VertexBuffer(vDesc);

CubeIL = new API::InputLayout();
CubeIL->Push(0, "POSITION", API::DataType::Float3, 5 * sizeof(float), 0);
CubeIL->Push(1, "TEXCOORD", API::DataType::Float2, 5 * sizeof(float), 3 * sizeof(float));

CubeVB->SetInputLayout(CubeIL, CubeShader);

Camera = new Components::FlyCamera();
Camera->Initialize(Math::Perspective(Math::Deg2Rad(45.0f), (float)800 / (float)600, 0.1f, 100.0f));
CubeShader->SetUniformBuffer(Camera->GetCBuffer() ,0, ShaderType::Vertex);

Texture_Desc Desc;
Desc.Filter = TextureFilter::Trilinear;
Desc.Wrap = TextureWrap::Repeat;
Desc.Format = TextureFormat::R8G8B8A8;

WoodenBoxTex = new API::Texture2D(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", Desc), Desc);

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
if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::Z))
{

}
else {
Camera->Update();
}
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

WoodenBoxTex->Bind(0);
CubeShader->Bind();
CubeVB->Bind();
Core::Context::Draw(36);
CubeVB->Unbind();
CubeShader->Unbind();
WoodenBoxTex->Unbind();

Core::Context::End();
}
void ExitRendering()	// Exit Rendering
{
delete CubeShader;
delete CubeVB;
delete WoodenBoxTex;
}
};

*/