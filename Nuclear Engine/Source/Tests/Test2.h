#pragma once
#include "TestCommon.h"

class Test2 : public Core::Game
{
protected:
	API::VertexShader VShader;
	API::PixelShader PShader;
	API::VertexBuffer RectangleVB;
	API::IndexBuffer RectangleIB;
	API::Texture WoodenBoxTex;
	API::Sampler WoodenBoxSampler;

	float vertices[20] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	std::string VertexShader = R"(struct VertexInputType
{
    float3 position : POSITION;
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

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = float4(input.position,  1);

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

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.tex);
}
)";

public:
	Test2()
	{
	}
	void Load()
	{
		//Load The Shader
		API::VertexShader::Create(
			&VShader,
			&API::CompileShader(VertexShader,
				API::ShaderType::Vertex));

		API::PixelShader::Create(
			&PShader,
			&API::CompileShader(PixelShader,
				API::ShaderType::Pixel));

		API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = API::BufferUsage::Dynamic;

		API::VertexBuffer::Create(&RectangleVB, Desc);

		API::InputLayout RectangleIL;
		RectangleIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		RectangleIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		RectangleVB.SetInputLayout(&RectangleIL, &VShader);
		API::IndexBuffer::Create(&RectangleIB,indices, sizeof(indices));

		//Create Texture Resource
		API::Texture_Desc TexDesc;
		TexDesc.Format = API::Format::R8G8B8A8_UNORM;
		TexDesc.Type = API::TextureType::Texture2D;
		TexDesc.GenerateMipMaps = false;
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, TexDesc);

		//Create Sampler
		API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = API::TextureFilter::Linear2D;
		API::Sampler::Create(&WoodenBoxSampler, Samplerdesc);

		Core::Application::Display();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override
	{
		//Change Background Color to Black in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		WoodenBoxTex.PSBind(0);
		WoodenBoxSampler.PSBind(0);
		RectangleVB.Bind();
		RectangleIB.Bind();
		Core::Context::DrawIndexed(6);

		Core::Context::PresentFrame();
	}
	void Shutdown() override 
	{
		API::VertexShader::Delete(&VShader);
		API::PixelShader::Delete(&PShader);
		API::VertexBuffer::Delete(&RectangleVB);
		API::IndexBuffer::Delete(&RectangleIB);
		API::Texture::Delete(&WoodenBoxTex);
	}
};
