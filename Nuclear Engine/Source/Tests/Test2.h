#pragma once
#include "TestCommon.h"

class Test2 : public Core::Game
{
protected:
	API::Shader RectangleShader;
	API::VertexBuffer RectangleVB;
	API::IndexBuffer RectangleIB;
	API::Texture WoodenBoxTex;
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
		API::ShaderDesc desc;
		desc.Name = "Test2";
		API::CompileShader(&desc.VertexShaderCode, VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&desc.PixelShaderCode, PixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);


		API::Shader::Create(&RectangleShader, &desc);

		API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = API::BufferUsage::Dynamic;

		API::VertexBuffer::Create(&RectangleVB, Desc);

		API::InputLayout RectangleIL;
		RectangleIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		RectangleIL.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

		RectangleVB.SetInputLayout(&RectangleIL, &RectangleShader);
		API::IndexBuffer::Create(&RectangleIB,indices, sizeof(indices));

		API::Texture_Desc TexDesc;
		TexDesc.Filter = API::TextureFilter::Linear2D;
		TexDesc.Wrap = API::TextureWrap::Repeat;
		TexDesc.Format = API::Format::R8G8B8A8;
		TexDesc.Type = API::TextureType::Texture2D;

		//AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, TexDesc);

		Core::Application::Display();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override
	{
		Core::Context::Begin();

		//Change Background Color to Black in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		WoodenBoxTex.PSBind(0);
		RectangleShader.Bind();
		RectangleVB.Bind();
		RectangleIB.Bind();
		Core::Context::DrawIndexed(6);

		Core::Context::End();
	}
	void Shutdown() override 
	{
		API::Shader::Delete(&RectangleShader);
		API::VertexBuffer::Delete(&RectangleVB);
		API::IndexBuffer::Delete(&RectangleIB);
		API::Texture::Delete(&WoodenBoxTex);
	}
};
