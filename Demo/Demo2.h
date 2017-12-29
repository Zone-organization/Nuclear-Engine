#pragma once
#include "Common.h"

class Demo2 : public Core::Game
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

public:
	Demo2()
	{
	}
	void Load()
	{

		API::Shader::Create(&RectangleShader,
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo2/Shaders/RectangleShader.vs").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/Demo2/Shaders/RectangleShader.ps").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));



		VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferGPUUsage::Dynamic;
		Desc.access = BufferCPUAccess::Default;

		API::VertexBuffer::Create(&RectangleVB, &Desc);

		API::InputLayout RectangleIL;
		RectangleIL.Push("POSITION", 0, DataType::Float3);
		RectangleIL.Push("TEXCOORD", 0, DataType::Float2);

		RectangleVB.SetInputLayout(&RectangleIL, &RectangleShader);
		API::IndexBuffer::Create(&RectangleIB,indices, sizeof(indices));

		Texture_Desc TexDesc;
		TexDesc.Filter = TextureFilter::Linear2D;
		TexDesc.Wrap = TextureWrap::Repeat;
		TexDesc.Format = TextureFormat::R8G8B8A8;
		TexDesc.Type = TextureType::Texture2D;
		API::Texture::Create(&WoodenBoxTex, &ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", TexDesc), &TexDesc);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Black in RGBA format
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));
	
		WoodenBoxTex.PSBind(0);
		RectangleShader.Bind();
		RectangleVB.Bind();
		RectangleIB.Bind();
		Core::Context::DrawIndexed(6);
		RectangleIB.Unbind();
		RectangleVB.Unbind();
		RectangleShader.Unbind();

		Core::Context::End();
	}

};
