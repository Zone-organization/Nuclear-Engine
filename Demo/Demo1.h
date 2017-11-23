#pragma once
#include "Common.h"

class Demo1 : public Core::Game
{
protected:
	API::Shader *TriangleShader;
	API::VertexBuffer *TriangleVB;
	API::InputLayout *TriangleIL;

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
		//Core::Context::EnableBlending(true);
		
		TriangleShader = new API::Shader("TriangleShader",
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo1/Shaders/DirectX/Triangle.vs").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo1/Shaders/DirectX/Triangle.ps").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));

		
		API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferGPUUsage::Dynamic;
		Desc.accessflag = BufferCPUAccess::Default;

		TriangleVB = new API::VertexBuffer(Desc);

		TriangleIL = new API::InputLayout();
		TriangleIL->Push("POSITION", 0, DataType::Float3, 6 * sizeof(float), 0);
		TriangleIL->Push("COLOR" , 0 , DataType::Float3, 6 * sizeof(float), 3 * sizeof(float));

		TriangleVB->SetInputLayout(TriangleIL, TriangleShader);


		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}
	float lastTime = 0;

	void Render() override		// Render The Game
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));
		//Don't Forget to clear the depth buffer each frame
		Core::Context::ClearDepthBuffer();

		// Render Triangle!
		TriangleShader->Bind();
		TriangleVB->Bind();
		Core::Context::Draw(3);
		TriangleVB->Unbind();
		TriangleShader->Unbind();

		Core::Context::End();
	}
	void ExitRendering() override	// Exit Rendering
	{
		delete TriangleShader;
		delete TriangleVB;
	}
};
