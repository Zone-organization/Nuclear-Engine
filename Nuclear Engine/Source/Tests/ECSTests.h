#pragma once
#include "TestCommon.h"

class ECSTests : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	Graphics::API::VertexShader Vertexshader;
	Graphics::API::PixelShader Pixelshader;

	Graphics::API::Sampler LinearSampler;
	Graphics::API::Texture PlaneTex;
	Graphics::API::Texture CubeTex;

	ECS::Scene Scene;
	ECS::Entity EPlane;
	ECS::Entity ECube1;
	ECS::Entity ECube2;

	Assets::Mesh Cube;
	Assets::Mesh Plane;

	Components::FlyCamera Camera;
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
	ECSTests()
	{
	}
	void Load()
	{
		Systems::RenderSystemDesc desc;
		Renderer = Scene.Systems.Add<Systems::RenderSystem>(desc);
		Scene.Systems.Configure();


		Graphics::API::VertexShader::Create(&Vertexshader, &Graphics::API::CompileShader(VertexShader, Graphics::API::ShaderType::Vertex));
		Graphics::API::PixelShader::Create(&Pixelshader, &Graphics::API::CompileShader(PixelShader, Graphics::API::ShaderType::Pixel));
		
		Camera.Initialize(Math::Perspective(Math::radians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Vertexshader.SetConstantBuffer(&Camera.GetCBuffer());

		Graphics::API::Texture_Desc Desc;
		Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
		Desc.Type = Graphics::API::TextureType::Texture2D;
		Desc.GenerateMipMaps = true;

		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &PlaneTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &CubeTex, Desc);

		Assets::MeshTexture MTexture;
		MTexture.type = Assets::MeshTextureType::Diffuse;
		MTexture.Texture = CubeTex;

		Assets::MeshVertexDesc vertexDesc;
		vertexDesc.Normals = false;
		vertexDesc.Tangents = false;
		Assets::Mesh::CreateCube(&Cube, std::vector<Assets::MeshTexture>() = { MTexture }, vertexDesc, 1.0f, 1.0f, 1.0f);

		MTexture.Texture = PlaneTex;
		Assets::Mesh::CreatePlane(&Plane, std::vector<Assets::MeshTexture>() = { MTexture }, vertexDesc, 2.0f, 2.0f, 2.0f,2.0f);
		
		ECube1 = Scene.Entities.Create();
		ECube1.Assign<Components::MeshComponent>(&Cube);
		ECube2 = Scene.Entities.Create();
		ECube2.Assign<Components::MeshComponent>(&Cube);
	
		//Create sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&LinearSampler, Samplerdesc);
		
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
		//PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4((1.0f)));
		Graphics::API::Context::Draw(6);

		Graphics::API::Context::PresentFrame();
	}
	void Shutdown() override
	{
		//Graphics::API::VertexBuffer::Delete(&CubeVB);
		//::API::VertexBuffer::Delete(&PlaneVB);
		Graphics::API::Texture::Delete(&CubeTex);
	}
};