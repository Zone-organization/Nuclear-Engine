#pragma once
#include "TestCommon.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Math/gtx/quaternion.hpp>
struct Shader_Uniforms_t {
	Math::Matrix4 MeshComponent = Math::Matrix4(1.0f);
	Math::Matrix4 View = Math::Matrix4(1.0f);
	Math::Matrix4 Projection = Math::Matrix4(1.0f);
}Shader_Uniforms;


class Test3 : public Core::Game
{
protected:
	Graphics::API::VertexShader VShader;
	Graphics::API::PixelShader PShader;

	Graphics::API::VertexBuffer CubeVB;
	Graphics::API::ConstantBuffer CubeCB;
	Graphics::API::Texture WoodenBoxTex;
	Graphics::API::Sampler WoodenBoxSampler;
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
	matrix View;
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(MeshComponent, input.position);
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
		Graphics::API::VertexShader::Create(
			&VShader,
			&Graphics::API::CompileShader(VertexShader,
				Graphics::API::ShaderType::Vertex));

		Graphics::API::PixelShader::Create(
			&PShader,
			&Graphics::API::CompileShader(PixelShader,
				Graphics::API::ShaderType::Pixel));
	
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

		Graphics::API::VertexBufferDesc vDesc;
		vDesc.data = vertices;
		vDesc.size = sizeof(vertices);
		vDesc.usage = Graphics::API::BufferUsage::Static;
		Graphics::API::VertexBuffer::Create(&CubeVB, vDesc);

		Graphics::API::InputLayout CubeIL;
		CubeIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		CubeIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		CubeVB.SetInputLayout(&CubeIL, &VShader);
		
		Graphics::API::ConstantBuffer::Create(&CubeCB, "NE_Camera", sizeof(Shader_Uniforms));
		VShader.SetConstantBuffer(&CubeCB);
	
		Graphics::API::Texture_Desc TexDesc;
		TexDesc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
		TexDesc.Type = Graphics::API::TextureType::Texture2D;
		TexDesc.GenerateMipMaps = true;
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, TexDesc);

		//Create Sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&WoodenBoxSampler, Samplerdesc);

		Shader_Uniforms.MeshComponent = Shader_Uniforms.MeshComponent * Math::toMat4(Math::Quaternion(Math::Vector3(0.5f, 1.0f, 0.0f)));
		Shader_Uniforms.View = Math::Translate(Shader_Uniforms.View, Math::Vector3(0.0f, 0.0f, -3.0f));
		Shader_Uniforms.Projection = Math::Perspective(45.0f, Core::Application::GetAspectRatiof(), 0.1f, 100.0f);

		CubeCB.Update(&Shader_Uniforms, sizeof(Shader_Uniforms));
		Core::Application::Display();

		states.EnabledDepth_DisabledStencil.Bind();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
	}

	void Render(float) override
	{


		//Change Background Color to Blue in RGBA format
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		WoodenBoxTex.PSBind(0);
		WoodenBoxSampler.PSBind(0);
		CubeVB.Bind();

		Graphics::API::Context::Draw(36);

		Graphics::API::Context::PresentFrame();
	}
	void Shutdown() override
	{
		Graphics::API::VertexShader::Delete(&VShader);
		Graphics::API::PixelShader::Delete(&PShader);
		Graphics::API::VertexBuffer::Delete(&CubeVB);
		Graphics::API::Texture::Delete(&WoodenBoxTex);
		Graphics::API::ConstantBuffer::Delete(&CubeCB);

	}
};
