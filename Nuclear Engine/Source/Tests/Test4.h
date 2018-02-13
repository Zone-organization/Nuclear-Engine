#pragma once
#include "TestCommon.h"
#include <iostream>

class Test4 : public Core::Game
{
protected:
	API::Shader TestShader;
	API::Shader DepthShader;

	API::VertexBuffer CubeVB;
	API::VertexBuffer PlaneVB;
	API::VertexBuffer WindowVB;

	API::Texture PlaneTex;
	API::Texture CubeTex;
	API::Texture WindowTex;

	bool Depthshaderenabled;

	Components::FlyCamera Camera;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;


	Math::Vector3 windows[5] = 
	{
		Math::Vector3(-1.5f, 0.0f, -0.48f),
		Math::Vector3(1.5f, 0.0f, 0.51f),
		Math::Vector3(0.0f, 0.0f, 0.7f),
		Math::Vector3(-0.3f, 0.0f, -2.3f),
		Math::Vector3(0.5f, 0.0f, -0.6f)
	};

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
	std::string DepthPixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D NE_Tex_Diffuse : register(t0);
SamplerState NE_Tex_Diffuse_Sampler : register(s0);

static const float far = 100.0f; 
static const float near = 0.1f;  

float LinearizeDepth(float depth) 
{
	
    float z = depth * 2.0f - 1.0f; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
float4 main(PixelInputType input) : SV_TARGET
{    
	float depth = LinearizeDepth(input.position.z) / 50.0f; // divide for demonstration
    return float4(float3(depth,depth,depth), 1.0f);
}
)";
	API::DepthStencilState DS_State;
	API::RasterizerState R_State;
	API::BlendState B_State;

public:
	Test4()
	{
	}
	void Load()
	{

		API::ShaderDesc desc;
		desc.Name = "TestShader";
		API::CompileShader(&desc.VertexShaderCode, VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&desc.PixelShaderCode, PixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);
		
		API::Shader::Create(&TestShader, &desc);

		API::ShaderDesc depthdesc;
		depthdesc.Name = "DepthShader";
		API::CompileShader(&depthdesc.VertexShaderCode, VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&depthdesc.PixelShaderCode, DepthPixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

		API::Shader::Create(&DepthShader, &depthdesc);

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
		float windowVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = BufferUsage::Static;
		API::VertexBuffer::Create(&CubeVB, &vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		API::VertexBuffer::Create(&PlaneVB, &vDesc);
		
		vDesc.data = windowVertices;
		vDesc.size = sizeof(windowVertices);
		API::VertexBuffer::Create(&WindowVB, &vDesc);

		API::InputLayout ShaderIL;
		ShaderIL.Push("POSITION", 0, DataType::Float3);
		ShaderIL.Push("TEXCOORD", 0, DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &TestShader);
		PlaneVB.SetInputLayout(&ShaderIL, &TestShader);
		WindowVB.SetInputLayout(&ShaderIL, &TestShader);

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		TestShader.SetConstantBuffer(&Camera.GetCBuffer(), API::ShaderType::Vertex);

		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &PlaneTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &CubeTex, Desc);
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/window.png", &WindowTex, Desc);

		API::DepthStencilStateDesc DS_Desc;
		DS_Desc.DepthEnabled = true;
		DS_Desc.DepthFunc = API::Comparison_Func::ALWAYS;
		API::DepthStencilState::Create(&DS_State, DS_Desc);

		API::RasterizerStateDesc rasterizerdesc;
		rasterizerdesc.FillMode = API::FillMode::Wireframe;

		API::RasterizerState::Create(&R_State, rasterizerdesc);

		API::BlendStateDesc blenddesc;
		blenddesc.RenderTarget[0].BlendEnable = true;
		blenddesc.RenderTarget[0].SrcBlend = API::BLEND::SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlend = API::BLEND::INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].SrcBlendAlpha = API::BLEND::SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlendAlpha = API::BLEND::INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].RenderTargetWriteMask = API::COLOR_WRITE_ENABLE_ALL;
		API::BlendState::Create(&B_State, blenddesc);
		
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
	0 - Visualize Depth Buffer
	1 - Enable Depth state
	2 - Restore Default Depth State
	3 - Enable Rasterizer State
	4 - Restore Default Rasterizer State
	5 - Enable Blending State
	6 - Restore Default Blending State
	
	W - Move Camera Forward
	A - Move Camera Left
	S - Move Camera Backward
	D - Move Camera Right

	Mouse - Make Camera look around
	*/

	void Render(float) override
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		TestShader.Bind();
		CubeTex.PSBind("NE_Tex_Diffuse", &TestShader, 0);
		
		
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num0))
		{
			DepthShader.Bind();
			Depthshaderenabled = true;
		}
		else {
			Depthshaderenabled = false;
		}

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		{
			DS_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num2)) 
		{
			API::DepthStencilState::Bind_Default();
			
		}

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num3))
		{
			R_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num4))
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
		if (!Depthshaderenabled)
		{
			PlaneTex.PSBind("NE_Tex_Diffuse", &TestShader, 0);
		}
	
		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4());
		Core::Context::Draw(6);

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num5))
		{
			B_State.Bind(API::Color(0.75f));
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num6))
		{
			B_State.Bind_Default();
		}

		if (!Depthshaderenabled)
		{
			WindowTex.PSBind("NE_Tex_Diffuse", &TestShader, 0);
		}

		WindowVB.Bind();
		std::map<float, Math::Vector3> sorted;
		for (unsigned int i = 0; i < 5; i++)
		{
			float distance = Math::Length(Camera.GetPosition() - windows[i]);
			sorted[distance] = windows[i];
		}
		for (std::map<float, Math::Vector3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			Math::Matrix4 model;
			model = Math::Translate(model, it->second);
			Camera.SetModelMatrix(model);
			Core::Context::Draw(6);
		}		
		ImGui::Render();
		Core::Context::End();
	}
	void Shutdown() override
	{
		API::Shader::Delete(&TestShader);
		API::VertexBuffer::Delete(&CubeVB);
		API::VertexBuffer::Delete(&PlaneVB);
		API::Texture::Delete(&CubeTex);
	}
};
