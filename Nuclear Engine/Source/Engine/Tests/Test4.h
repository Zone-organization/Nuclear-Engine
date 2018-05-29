#pragma once
#include "TestCommon.h"
#include <iostream>
#include <map>
class Test4 : public Core::Game
{
protected:
	Graphics::API::VertexShader Vertexshader;
	Graphics::API::PixelShader Pixelshader;
	Graphics::API::PixelShader Depthpixelshader;

	Graphics::API::VertexBuffer CubeVB;
	Graphics::API::VertexBuffer PlaneVB;
	Graphics::API::VertexBuffer WindowVB;

	Graphics::API::Sampler LinearSampler;
	Assets::Texture PlaneTex;
	Assets::Texture CubeTex;
	Assets::Texture WindowTex;

	bool Depthshaderenabled = false;

	Components::CameraComponent Camera;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	//Default states
	Graphics::API::CommonStates states;


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
	Graphics::API::DepthStencilState DS_State;
	Graphics::API::RasterizerState R_State;
	Graphics::API::BlendState B_State;

public:
	Test4()
	{
	}
	void Load()
	{
		Graphics::API::VertexShader::Create(&Vertexshader, &Graphics::API::CompileShader(VertexShader, Graphics::API::ShaderType::Vertex));
		Graphics::API::PixelShader::Create(&Pixelshader, &Graphics::API::CompileShader(PixelShader, Graphics::API::ShaderType::Pixel));
		Graphics::API::PixelShader::Create(&Depthpixelshader, &Graphics::API::CompileShader(DepthPixelShader, Graphics::API::ShaderType::Pixel));

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

		Graphics::API::VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = Graphics::API::BufferUsage::Static;
		Graphics::API::VertexBuffer::Create(&CubeVB, vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		Graphics::API::VertexBuffer::Create(&PlaneVB, vDesc);
		
		vDesc.data = windowVertices;
		vDesc.size = sizeof(windowVertices);
		Graphics::API::VertexBuffer::Create(&WindowVB, vDesc);

		Graphics::API::InputLayout ShaderIL;
		ShaderIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		ShaderIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &Vertexshader);
		PlaneVB.SetInputLayout(&ShaderIL, &Vertexshader);
		WindowVB.SetInputLayout(&ShaderIL, &Vertexshader);

		Camera.Initialize(Math::Perspective(Math::radians(45.0f), static_cast<Float32>(Core::Application::GetAspectRatio()), 0.1f, 100.0f));

		Vertexshader.SetConstantBuffer(&Camera.GetCBuffer());

		PlaneTex = Managers::AssetManager::Import("Assets/Common/Textures/woodenbox.jpg");
		CubeTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_diffuse.png");
		WindowTex= Managers::AssetManager::Import("Assets/Common/Textures/window.png");

		//Create sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&LinearSampler, Samplerdesc);

		Graphics::API::DepthStencilStateDesc DS_Desc;
		DS_Desc.DepthEnabled = true;
		DS_Desc.DepthFunc = Graphics::API::Comparison_Func::ALWAYS;
		Graphics::API::DepthStencilState::Create(&DS_State, DS_Desc);

		Graphics::API::RasterizerStateDesc rasterizerdesc;
		rasterizerdesc.FillMode = Graphics::API::FillMode::Wireframe;

		Graphics::API::RasterizerState::Create(&R_State, rasterizerdesc);

		Graphics::API::BlendStateDesc blenddesc;
		blenddesc.RenderTarget[0].BlendEnable = true;
		blenddesc.RenderTarget[0].SrcBlend = Graphics::API::BLEND::SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlend = Graphics::API::BLEND::INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].SrcBlendAlpha = Graphics::API::BLEND::SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlendAlpha = Graphics::API::BLEND::INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].RenderTargetWriteMask = Graphics::API::COLOR_WRITE_ENABLE_ALL;
		Graphics::API::BlendState::Create(&B_State, blenddesc);
		
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
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
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::D))
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
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		Vertexshader.Bind();		
		
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num0))
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

		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num1))
		{
			DS_State.Bind();
		}
		else if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num2)) 
		{
			states.DefaultDepthStencil.Bind();
		}

		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num3))
		{
			R_State.Bind();
		}
		else if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num4))
		{
			states.DefaultRasterizer.Bind();
		}

		CubeVB.Bind();

		// cube 1
		Math::Matrix4 CubeModel(1.0f);
		CubeModel = Math::Translate(CubeModel, Math::Vector3(-1.0f, 0.0f, -1.0f));
		Camera.SetModelMatrix(CubeModel);
		Graphics::API::Context::Draw(36);
		// cube 2
		CubeModel = Math::Matrix4(1.0f);
		CubeModel = Math::Translate(CubeModel, Math::Vector3(2.0f, 0.0f, 0.0f));
		Camera.SetModelMatrix(CubeModel);
		Graphics::API::Context::Draw(36);
		
		// floor
		if (!Depthshaderenabled)
		{
			PlaneTex.PSBind(0);
		}
	
		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4((1.0f)));
		Graphics::API::Context::Draw(6);

		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num5))
		{
			B_State.Bind(Graphics::Color(0.75f));
		}
		else if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num6))
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
			Graphics::API::Context::Draw(6);
		}		

		Graphics::API::Context::PresentFrame();
	}
	void Shutdown() override
	{
		//Graphics::API::Shader::Delete(&TestShader);
		Graphics::API::VertexBuffer::Delete(&CubeVB);
		Graphics::API::VertexBuffer::Delete(&PlaneVB);
	}
};
