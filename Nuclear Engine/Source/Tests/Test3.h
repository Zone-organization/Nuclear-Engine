#pragma once
#include "TestCommon.h"

struct Shader_Uniforms_t {
	Math::Matrix4 Model;
	Math::Matrix4 View;
	Math::Matrix4 Projection;
}Shader_Uniforms;


class Test3 : public Core::Game
{
protected:
	API::VertexShader VShader;
	API::PixelShader PShader;

	API::VertexBuffer CubeVB;
	API::ConstantBuffer CubeCB;
	API::Texture WoodenBoxTex;
	API::Sampler WoodenBoxSampler;
	API::CommonStates states;
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
public:
	Test3()
	{
	}
	void Load()
	{

		//Load The Shader
		API::VertexShader::Create(
			&VShader,
			&API::CompileShader(VertexShader,
				API::ShaderType::Vertex,
				API::ShaderLanguage::HLSL));

		API::PixelShader::Create(
			&PShader,
			&API::CompileShader(PixelShader,
				API::ShaderType::Pixel,
				API::ShaderLanguage::HLSL));
	
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
		vDesc.usage = API::BufferUsage::Static;
		API::VertexBuffer::Create(&CubeVB, vDesc);

		API::InputLayout CubeIL;
		CubeIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		CubeIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		CubeVB.SetInputLayout(&CubeIL, &VShader);
		
		API::ConstantBuffer::Create(&CubeCB, "NE_Camera", sizeof(Shader_Uniforms));
		VShader.SetConstantBuffer(&CubeCB);
	
		API::Texture_Desc TexDesc;
		TexDesc.Format = API::Format::R8G8B8A8_UNORM;
		TexDesc.Type = API::TextureType::Texture2D;
		TexDesc.GenerateMipMaps = true;
		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, TexDesc);

		//Create Sampler
		API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = API::TextureFilter::Trilinear;
		API::Sampler::Create(&WoodenBoxSampler, Samplerdesc);

		Shader_Uniforms.Model = Math::Rotate(Math::Vector3(0.5f, 1.0f, 0.0f), 5.0f);
		Shader_Uniforms.View = Math::Translate(Math::Vector3(0.0f, 0.0f, -3.0f));
		Shader_Uniforms.Projection = Math::Perspective(45.0f, Core::Application::GetAspectRatiof(), 0.1f, 100.0f);

		CubeCB.Update(&Shader_Uniforms, sizeof(Shader_Uniforms));
		Core::Application::Display();

		states.EnabledDepth_DisabledStencil.Bind();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		WoodenBoxTex.PSBind(0);
		WoodenBoxSampler.PSBind(0);
		CubeVB.Bind();

		Core::Context::Draw(36);

		Core::Context::End();
	}
	void Shutdown() override
	{
		API::VertexShader::Delete(&VShader);
		API::PixelShader::Delete(&PShader);
		API::VertexBuffer::Delete(&CubeVB);
		API::Texture::Delete(&WoodenBoxTex);
		API::ConstantBuffer::Delete(&CubeCB);

	}
};
