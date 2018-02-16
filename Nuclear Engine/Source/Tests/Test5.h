#pragma once
#include "TestCommon.h"
#include <iostream>

class Test5 : public Core::Game
{
protected:
	API::VertexShader Vertexshader;
	API::PixelShader Pixelshader;
	API::VertexShader ScreenVertexshader;
	API::PixelShader ScreenPixelshader;

	API::VertexBuffer CubeVB;
	API::VertexBuffer PlaneVB;
	API::VertexBuffer ScreenQuadVB;

	API::Texture PlaneTex;
	API::Texture CubeTex;
	API::Texture ScreenTex;

	API::RenderTarget RT;

	Components::FlyCamera Camera;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	std::string VertexShader = R"(struct VertexInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer NE_Camera : register(b0)
{
	matrix Model;
	matrix View;
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(Model, input.position);
	output.position = mul(View, output.position);
	output.position = mul(Projection, output.position);

	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    return output;
})";

	std::string PixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D NE_Tex_Diffuse : register(t0);
SamplerState NE_Tex_Diffuse_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sampler, input.tex);
}
)";
	std::string ScreenVertexShader = R"(struct VertexInputType
{
    float2 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// CalcSulate the position of the vertex against the world, view, and projection matrices.
	output.position = float4(input.position.x,input.position.y,0.0f,1.0f);
	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    return output;
})";

	std::string ScreenPixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D ScreenTexture : register(t0);
SamplerState ScreenTexture_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return float4(ScreenTexture.Sample(ScreenTexture_Sampler, input.tex).rgb, 1.0f);
}
)";

	API::RasterizerState R_State;
public:
	Test5()
	{
	}
	void Load()
	{
		API::VertexShader::Create(&Vertexshader, &API::CompileShader(VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL, true, true));
		API::PixelShader::Create(&Pixelshader, &API::CompileShader(PixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL, true, true));
		API::VertexShader::Create(&ScreenVertexshader, &API::CompileShader(ScreenVertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL, true, true));
		API::PixelShader::Create(&ScreenPixelshader, &API::CompileShader(ScreenPixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL, true, true));

		float cubevertices[] = {
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
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
	
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		API::VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = API::BufferUsage::Static;
		API::VertexBuffer::Create(&CubeVB, vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		API::VertexBuffer::Create(&PlaneVB, vDesc);

		vDesc.data = quadVertices;
		vDesc.size = sizeof(quadVertices);
		API::VertexBuffer::Create(&ScreenQuadVB, vDesc);

		API::InputLayout ShaderIL;
		ShaderIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		ShaderIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &Vertexshader);
		PlaneVB.SetInputLayout(&ShaderIL, &Vertexshader);

		API::InputLayout ScreenShaderIL;
		ScreenShaderIL.AppendAttribute("POSITION", 0, API::DataType::Float2);
		ScreenShaderIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		ScreenQuadVB.SetInputLayout(&ScreenShaderIL, &ScreenVertexshader);


		int windowwidth, windowheight;
		Core::Application::GetSize(&windowwidth, &windowheight);


		API::Texture_Desc ScreenTexDesc;
		ScreenTexDesc.Filter = API::TextureFilter::Linear2D;
		ScreenTexDesc.Wrap = API::TextureWrap::Repeat;
		ScreenTexDesc.Format = API::Format::R8G8B8A8_UNORM;
		ScreenTexDesc.Type = API::TextureType::Texture2D;
		ScreenTexDesc.RenderTarget = true;

		API::Texture_Data Data;
		Data.Img_Data_Buf = NULL;
		Data.Width = windowwidth;
		Data.Height = windowheight;
		API::Texture::Create(&ScreenTex, Data, ScreenTexDesc);

		//RT
		API::RenderTarget::Create(&RT);
		RT.AttachTexture(&ScreenTex);
		RT.AttachDepthStencilBuffer((windowwidth, windowheight));

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Vertexshader.SetConstantBuffer(&Camera.GetCBuffer());

		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8_UNORM;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &PlaneTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &CubeTex, Desc);

	
		API::RasterizerStateDesc rasterizerdesc;
		rasterizerdesc.FillMode = API::FillMode::Wireframe;

		API::RasterizerState::Create(&R_State, rasterizerdesc);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
		Core::Context::EnableDepthBuffer(true);

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

	/*
	Controls:
	1 - Enable Rasterizer State
	2 - Restore Default Rasterizer State

	W - Move Camera Forward
	A - Move Camera Left
	S - Move Camera Backward
	D - Move Camera Right

	Mouse - Make Camera look around
	*/

	void Render(float) override
	{
		Core::Context::Begin();

		//Bind The RenderTarget
		RT.Bind();

		//Enable Depth Test
		Core::Context::EnableDepthBuffer(true);
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);


		Vertexshader.Bind();

		Pixelshader.Bind();
		CubeTex.SetInShader("NE_Tex_Diffuse", &Pixelshader, 0);
		

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		{
			R_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num2))
		{
			R_State.Bind_Default();
		}

		CubeVB.Bind();

		// cube 1
		Math::Matrix4 CubeModel;
		CubeModel = Math::Translate(CubeModel, Math::Vector3(-1.0f, 0.0f, -1.0f));
		Camera.SetModelMatrix(CubeModel);
		Core::Context::Draw(36);
		// cube 2
		CubeModel = Math::Matrix4();
		CubeModel = Math::Translate(CubeModel, Math::Vector3(2.0f, 0.0f, 0.0f));
		Camera.SetModelMatrix(CubeModel);
		Core::Context::Draw(36);

		// floor
		PlaneTex.SetInShader("NE_Tex_Diffuse", &Pixelshader, 0);		

		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4());
		Core::Context::Draw(6);		

		//Bind default RenderTarget
		RT.Bind();
		Core::Context::EnableDepthBuffer(false);
		Core::Context::Clear(API::Color(1.0f, 1.0f, 1.0f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		{
			R_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num2))
		{
			R_State.Bind_Default();
		}

		ScreenVertexshader.Bind();
		ScreenPixelshader.Bind();
		ScreenQuadVB.Bind();
		ScreenTex.SetInShader("ScreenTexture", &ScreenPixelshader, 0);
		Core::Context::Draw(6);


		Core::Context::End();
	}
	void Shutdown() override
	{
		//API::Shader::Delete(&TestShader);
		API::VertexBuffer::Delete(&CubeVB);
		API::VertexBuffer::Delete(&PlaneVB);
		API::Texture::Delete(&CubeTex);
	}
};
