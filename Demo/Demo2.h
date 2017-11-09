#pragma once
#include "Common.h"

class Demo2 : public Core::Game
{
protected:
	API::Shader *RectangleShader;
	API::VertexBuffer *RectangleVB;
	API::IndexBuffer *RectangleIB;
	API::InputLayout *RectangleIL;
	API::Texture2D *WoodenBoxTex;
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

public:
	Demo2()
	{
	}
	void Load()
	{

		RectangleShader = new API::Shader("RectangleShader",
			API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo2/Shaders/DirectX/RectangleShader.vs").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo2/Shaders/DirectX/RectangleShader.ps").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));


		API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferGPUUsage::Dynamic;
		Desc.accessflag = BufferCPUAccess::Default;

		RectangleVB = new API::VertexBuffer(Desc);

		RectangleIL = new API::InputLayout();
		RectangleIL->Push(0, "POSITION", API::DataType::Float3, 5 * sizeof(float), 0);
		RectangleIL->Push(1, "TEXCOORD", API::DataType::Float2, 5 * sizeof(float), 3 * sizeof(float));

		RectangleVB->SetInputLayout(RectangleIL, RectangleShader);
		RectangleIB = new API::IndexBuffer(indices, sizeof(indices));

		Texture_Desc TexDesc;
		TexDesc.Filter = TextureFilter::Linear2D;
		TexDesc.Wrap = TextureWrap::Repeat;
		TexDesc.Format = TextureFormat::R8G8B8A8;

		WoodenBoxTex = new API::Texture2D(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", TexDesc), TexDesc);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Black in RGBA format
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));


		WoodenBoxTex->Bind(0);
		RectangleShader->Bind();
		RectangleVB->Bind();
		RectangleIB->Bind();
		Core::Context::DrawIndexed(6);
		RectangleIB->Unbind();
		RectangleVB->Unbind();
		RectangleShader->Unbind();
		WoodenBoxTex->Unbind();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
		delete RectangleShader;
		delete RectangleVB;
		delete RectangleIB;
		delete WoodenBoxTex;
	}
};
