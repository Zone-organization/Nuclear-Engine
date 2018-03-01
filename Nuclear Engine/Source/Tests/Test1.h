#pragma once
#include "TestCommon.h"

class Test1 : public Core::Game
{
protected:
	API::VertexShader VShader;
	API::PixelShader PShader;

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
		
		API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = API::BufferUsage::Dynamic;

		//Create the vertex Buffer
		API::VertexBuffer::Create(&TriangleVB,Desc);

		API::InputLayout TriangleIL;
		TriangleIL.AppendAttribute("POSITION", 0, API::DataType::Float3);
		TriangleIL.AppendAttribute("COLOR" , 0 , API::DataType::Float3);

		TriangleVB.SetInputLayout(&TriangleIL, &VShader);

		Core::Application::Display();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override		// Render The Game
	{


		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		TriangleVB.Bind();
		Core::Context::Draw(3);

		Core::Context::PresentFrame();
	}

	void Shutdown() override 
	{
		API::VertexShader::Delete(&VShader);
		API::PixelShader::Delete(&PShader);
		API::VertexBuffer::Delete(&TriangleVB);
	}
};
