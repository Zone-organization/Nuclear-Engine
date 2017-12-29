#pragma once
#include "Common.h"

class Demo1 : public Core::Game
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

public:
	Demo1()
	{
	}
	void Load() override
	{
		//Load The Shader
		API::Shader::Create(&TriangleShader,
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo1/Shaders/Triangle.vs").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo1/Shaders/Triangle.ps").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));

		
		VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferGPUUsage::Dynamic;
		Desc.access = BufferCPUAccess::Default;

		//Create the vertex Buffer
		API::VertexBuffer::Create(&TriangleVB,&Desc);

		API::InputLayout TriangleIL;
		TriangleIL.Push("POSITION", 0, DataType::Float3);
		TriangleIL.Push("COLOR" , 0 , DataType::Float3);

		TriangleVB.SetInputLayout(&TriangleIL, &TriangleShader);


		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}
	float lastTime = 0;

	void Render() override		// Render The Game
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));

		TriangleShader.Bind();
		TriangleVB.Bind();
		Core::Context::Draw(3);
		TriangleVB.Unbind();
		TriangleShader.Unbind();

		Core::Context::End();
	}
};
