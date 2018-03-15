#pragma once
#include "TestCommon.h"
#include <iostream>
#include <map>
class Test4 : public Core::Game
{
protected:
	API::VertexShader Vertexshader;
	API::PixelShader Pixelshader;
	API::PixelShader Depthpixelshader;

	API::VertexBuffer CubeVB;
	API::VertexBuffer PlaneVB;
	API::VertexBuffer WindowVB;

	API::Sampler LinearSampler;
	API::Texture PlaneTex;
	API::Texture CubeTex;
	API::Texture WindowTex;

	bool Depthshaderenabled = false;

	Components::FlyCamera Camera;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	//Default states
	API::CommonStates states;


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
    matrix ModelInvTranspose;
    matrix ModelViewProjection;
    matrix View;
    matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(ModelViewProjection, input.position);

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
		API::VertexShader::Create(&Vertexshader, &API::CompileShader(VertexShader, API::ShaderType::Vertex));
		API::PixelShader::Create(&Pixelshader, &API::CompileShader(PixelShader, API::ShaderType::Pixel));
		API::PixelShader::Create(&Depthpixelshader, &API::CompileShader(DepthPixelShader, API::ShaderType::Pixel));

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

		API::VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = API::BufferUsage::Static;
		API::VertexBuffer::Create(&CubeVB, vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		API::VertexBuffer::Create(&PlaneVB, vDesc);
		
		vDesc.data = windowVertices;
		vDesc.size = sizeof(windowVertices);
		API::VertexBuffer::Create(&WindowVB, vDesc);

		API::InputLayout ShaderIL;
		ShaderIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		ShaderIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &Vertexshader);
		PlaneVB.SetInputLayout(&ShaderIL, &Vertexshader);
		WindowVB.SetInputLayout(&ShaderIL, &Vertexshader);

		Camera.Initialize(Math::Perspective(Math::radians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Vertexshader.SetConstantBuffer(&Camera.GetCBuffer());

		API::Texture_Desc Desc;
		Desc.Format = API::Format::R8G8B8A8_UNORM;
		Desc.Type = API::TextureType::Texture2D;
		Desc.GenerateMipMaps = true;
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &PlaneTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &CubeTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/window.png", &WindowTex, Desc);

		//Create sampler
		API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = API::TextureFilter::Trilinear;
		API::Sampler::Create(&LinearSampler, Samplerdesc);

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
		states.EnabledDepth_DisabledStencil.Bind();

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


		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		Vertexshader.Bind();		
		
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num0))
		{
			Depthpixelshader.Bind();
			Depthshaderenabled = true;
		}
		else
		{
			Pixelshader.Bind();
			LinearSampler.PSBind(0);
			CubeTex.PSBind(0);
			Depthshaderenabled = false;
		}

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		{
			DS_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num2)) 
		{
			states.DefaultDepthStencil.Bind();
		}

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num3))
		{
			R_State.Bind();
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num4))
		{
			states.DefaultRasterizer.Bind();
		}

		CubeVB.Bind();

		// cube 1
		Math::Matrix4 CubeModel(1.0f);
		CubeModel = Math::Translate(CubeModel, Math::Vector3(-1.0f, 0.0f, -1.0f));
		Camera.SetModelMatrix(CubeModel);
		Core::Context::Draw(36);
		// cube 2
		CubeModel = Math::Matrix4(1.0f);
		CubeModel = Math::Translate(CubeModel, Math::Vector3(2.0f, 0.0f, 0.0f));
		Camera.SetModelMatrix(CubeModel);
		Core::Context::Draw(36);
		
		// floor
		if (!Depthshaderenabled)
		{
			PlaneTex.PSBind(0);
		}
	
		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4((1.0f)));
		Core::Context::Draw(6);

		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num5))
		{
			B_State.Bind(API::Color(0.75f));
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num6))
		{
			states.DefaultBlendState.Bind();
		}

		if (!Depthshaderenabled)
		{
			WindowTex.PSBind(0);
		}

		WindowVB.Bind();
		std::map<float, Math::Vector3> sorted;
		for (unsigned int i = 0; i < 5; i++)
		{
			float distance = Math::length(Camera.GetPosition() - windows[i]);
			sorted[distance] = windows[i];
		}
		for (std::map<float, Math::Vector3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			Math::Matrix4 model(1.0f);
			model = Math::Translate(model, it->second);
			Camera.SetModelMatrix(model);
			Core::Context::Draw(6);
		}		

		Core::Context::PresentFrame();
	}
	void Shutdown() override
	{
		//API::Shader::Delete(&TestShader);
		API::VertexBuffer::Delete(&CubeVB);
		API::VertexBuffer::Delete(&PlaneVB);
		API::Texture::Delete(&CubeTex);
	}
};
