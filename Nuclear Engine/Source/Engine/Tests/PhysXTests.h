#pragma once
#include "TestCommon.h"

#include <PhysX\PhysXincluder.h>
#ifndef _DEBUG 
class PhysXTests : public Core::Game
{
};
#elif defined(PHYSX_NOT_INCLUDED)
class PhysXTests : public Core::Game
{
};
#else


class PhysXTests : public Core::Game
{
protected:
	Graphics::API::VertexShader Vertexshader;
	Graphics::API::PixelShader Pixelshader;

	Graphics::API::VertexBuffer CubeVB;
	Graphics::API::VertexBuffer PlaneVB;

	Graphics::API::Sampler LinearSampler;
	Assets::Texture PlaneTex;
	Assets::Texture CubeTex;

	Physics3D::PhysicsScene scene;

	Components::CameraComponent Camera;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	//Default states
	Graphics::API::CommonStates states;

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
	
public:
	PhysXTests()
	{
	}
	void Load()
	{
		Graphics::API::VertexShader::Create(&Vertexshader, &Graphics::API::CompileShader(VertexShader, Graphics::API::ShaderType::Vertex));
		Graphics::API::PixelShader::Create(&Pixelshader, &Graphics::API::CompileShader(PixelShader, Graphics::API::ShaderType::Pixel));

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
	
		Graphics::API::VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = Graphics::API::BufferUsage::Static;
		Graphics::API::VertexBuffer::Create(&CubeVB, vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		Graphics::API::VertexBuffer::Create(&PlaneVB, vDesc);

		Graphics::API::InputLayout ShaderIL;
		ShaderIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		ShaderIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &Vertexshader);
		PlaneVB.SetInputLayout(&ShaderIL, &Vertexshader);

		Camera.Initialize(Math::Perspective(Math::radians(45.0f), static_cast<Float32>(Core::Application::GetAspectRatio()), 0.1f, 100.0f));

		Vertexshader.SetConstantBuffer(&Camera.GetCBuffer());
		
		PlaneTex = Managers::AssetManager::Import("Assets/Common/Textures/woodenbox.jpg");
		CubeTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_diffuse.png");

		//Create sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&LinearSampler, Samplerdesc);

		//Physics
		Physics3D::Physics3DEngine::Initialize();

		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
		states.EnabledDepth_DisabledStencil.Bind();

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void OnMouseMovement(int xpos_a, int ypos_a) override
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
		Pixelshader.Bind();
		LinearSampler.PSBind(0);
		CubeTex.PSBind(0);

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
		PlaneTex.PSBind(0);
		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4((1.0f)));
		Graphics::API::Context::Draw(6);

		Graphics::API::Context::PresentFrame();
	}
	void Shutdown() override
	{
		Graphics::API::VertexBuffer::Delete(&CubeVB);
		Graphics::API::VertexBuffer::Delete(&PlaneVB);
	}
};
#endif