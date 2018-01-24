#pragma once
#include "TestCommon.h"
#include <iostream>
struct Shader_Uniforms_t {
	Math::Matrix4 Model;
	Math::Matrix4 View;
	Math::Matrix4 Projection;
}Shader_Uniforms;


class Test3 : public Core::Game
{
protected:
	API::Shader CubeShader;
	API::VertexBuffer CubeVB;
	API::ConstantBuffer CubeCB;
	API::Texture WoodenBoxTex;

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

		API::ShaderDesc desc;
		desc.Name = "Test3";
		API::CompileShader(&desc.VertexShaderCode, VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&desc.PixelShaderCode, PixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

		API::Shader::Create(&CubeShader, &desc);
	
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

		VertexBufferDesc vDesc;
		vDesc.data = vertices;
		vDesc.size = sizeof(vertices);
		vDesc.usage = BufferUsage::Static;
		API::VertexBuffer::Create(&CubeVB, &vDesc);

		API::InputLayout CubeIL;
		CubeIL.Push("POSITION", 0, DataType::Float3);
		CubeIL.Push("TEXCOORD", 0, DataType::Float2);

		CubeVB.SetInputLayout(&CubeIL, &CubeShader);
		
		API::ConstantBuffer::Create(&CubeCB, "NE_Camera", sizeof(Shader_Uniforms));
		CubeShader.SetConstantBuffer(&CubeCB,API::ShaderType::Vertex);
	
		API::Texture_Desc Desc;
		Desc.Filter = API::TextureFilter::Trilinear;
		Desc.Wrap = API::TextureWrap::Repeat;
		Desc.Format = API::Format::R8G8B8A8;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, Desc);

		Shader_Uniforms.Model = Math::Rotate(Math::Vector3(0.5f, 1.0f, 0.0f), 5.0f);
		Shader_Uniforms.View = Math::Translate(Math::Vector3(0.0f, 0.0f, -3.0f));
		Shader_Uniforms.Projection = Math::Perspective(45.0f, Core::Application::GetAspectRatio(), 0.1f, 100.0f);

		CubeCB.Update(&Shader_Uniforms, sizeof(Shader_Uniforms));
		Core::Application::Display();

		Core::Context::EnableDepthBuffer(true);
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		WoodenBoxTex.PSBind(0);
		CubeShader.Bind();
		CubeVB.Bind();

		Core::Context::Draw(36);

		Core::Context::End();
	}
	void Shutdown() override
	{
		API::Shader::Delete(&CubeShader);
		API::VertexBuffer::Delete(&CubeVB);
		API::Texture::Delete(&WoodenBoxTex);
		API::ConstantBuffer::Delete(&CubeCB);

	}
};
