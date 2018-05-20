#pragma once
#include "TestCommon.h"

class Test2 : public Core::Game
{
protected:
	Graphics::API::VertexShader VShader;
	Graphics::API::PixelShader PShader;
	Graphics::API::VertexBuffer RectangleVB;
	Graphics::API::IndexBuffer RectangleIB;
	Assets::Texture WoodenBoxTex;
	Graphics::API::Sampler WoodenBoxSampler;

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
		Graphics::API::VertexShader::Create(
			&VShader,
			&Graphics::API::CompileShader(VertexShader,
				Graphics::API::ShaderType::Vertex));

		Graphics::API::PixelShader::Create(
			&PShader,
			&Graphics::API::CompileShader(PixelShader,
				Graphics::API::ShaderType::Pixel));

		Graphics::API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = Graphics::API::BufferUsage::Dynamic;

		Graphics::API::VertexBuffer::Create(&RectangleVB, Desc);

		Graphics::API::InputLayout RectangleIL;
		RectangleIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		RectangleIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		RectangleVB.SetInputLayout(&RectangleIL, &VShader);
		Graphics::API::IndexBuffer::Create(&RectangleIB,indices, sizeof(indices));

		//Create Texture Resource
		Graphics::API::Texture_Desc TexDesc;
		TexDesc.GenerateMipMaps = false;
		WoodenBoxTex = Managers::AssetManager::Import("Assets/Common/Textures/woodenbox.jpg", TexDesc);

		//Create Sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Linear2D;
		Graphics::API::Sampler::Create(&WoodenBoxSampler, Samplerdesc);

		Core::Application::Display();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
	}

	void Render(float) override
	{
		//Change Background Color to Black in RGBA format
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		WoodenBoxTex.PSBind(0, WoodenBoxSampler);
		RectangleVB.Bind();
		RectangleIB.Bind();
		Graphics::API::Context::DrawIndexed(6);

		Graphics::API::Context::PresentFrame();
	}
	void Shutdown() override 
	{
		Graphics::API::VertexShader::Delete(&VShader);
		Graphics::API::PixelShader::Delete(&PShader);
		Graphics::API::VertexBuffer::Delete(&RectangleVB);
		Graphics::API::IndexBuffer::Delete(&RectangleIB);
	}
};
