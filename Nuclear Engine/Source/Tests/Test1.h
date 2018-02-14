#pragma once
#include "TestCommon.h"

class Test1 : public Core::Game
{
protected:
	API::Shader TriangleShader;
	API::VertexBuffer TriangleVB;

	float vertices[18] = 
	{
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};

	std::string VertexShader = R"(struct VertexInputType
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.position = float4(input.position, 1.0f);
	output.color = input.color;
	return output;
})";

	std::string PixelShader = R"(
struct PixelInputType
{
	float4 position : SV_POSITION;
float3 color : COLOR;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return float4(input.color, 1);
}
)";

public:
	Test1()
	{
	}
	void Load() override
	{
		//Load The Shader
		API::ShaderDesc desc;
		desc.Name = "Test1";	
		API::CompileShader(&desc.VertexShaderCode, VertexShader, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
		API::CompileShader(&desc.PixelShaderCode, PixelShader, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

		API::Shader::Create(&TriangleShader, &desc);
		
		
		VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferUsage::Dynamic;

		//Create the vertex Buffer
		API::VertexBuffer::Create(&TriangleVB,&Desc);

		API::InputLayout TriangleIL;
		TriangleIL.Push("POSITION", 0, DataType::Float3);
		TriangleIL.Push("COLOR" , 0 , DataType::Float3);

		TriangleVB.SetInputLayout(&TriangleIL, &TriangleShader);

		Core::Application::Display();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override		// Render The Game
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		TriangleShader.Bind();
		TriangleVB.Bind();
		Core::Context::Draw(3);

		Core::Context::End();
	}

	void Shutdown() override 
	{
		API::Shader::Delete(&TriangleShader);
		API::VertexBuffer::Delete(&TriangleVB);
	}
};
