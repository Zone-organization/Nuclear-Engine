#pragma once
#include "TestCommon.h"

class Test1 : public Core::Game
{
protected:
	Graphics::API::VertexShader VShader;
	Graphics::API::PixelShader PShader;

	Graphics::API::VertexBuffer TriangleVB;

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

		//Create the vertex Buffer
		Graphics::API::VertexBuffer::Create(&TriangleVB,Desc);

		Graphics::API::InputLayout TriangleIL;
		TriangleIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		TriangleIL.AppendAttribute("COLOR" , 0 , Graphics::API::DataType::Float3);

		TriangleVB.SetInputLayout(&TriangleIL, &VShader);

		Core::Application::Display();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
	}

	void Render(float) override		// Render The Game
	{


		//Change Background Color to Blue in RGBA format
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		TriangleVB.Bind();
		Graphics::API::Context::Draw(3);

		Graphics::API::Context::PresentFrame();
	}

	void Shutdown() override 
	{
		Graphics::API::VertexShader::Delete(&VShader);
		Graphics::API::PixelShader::Delete(&PShader);
		Graphics::API::VertexBuffer::Delete(&TriangleVB);
	}
};
