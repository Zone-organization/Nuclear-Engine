#pragma once
#include "Common.h"

struct Shader_Uniforms_t {
	Math::Matrix4 Model;
	Math::Matrix4 View;
	Math::Matrix4 Projection;
}Shader_Uniforms;

class Demo3 : public Core::Game
{
protected:
	API::Shader *CubeShader;
	API::VertexBuffer *CubeVB;
	API::UniformBuffer *CubeCB;
	API::InputLayout *CubeIL;
	API::Texture2D *WoodenBoxTex;
	
public:
	Demo3()
	{
	}
	void Load()
	{
		CubeShader = new API::Shader("CubeShader",Core::FileSystem::LoadFileToString("Assets/Demo3/Shaders/DirectX/CubeShader.vs").c_str(),
			Core::FileSystem::LoadFileToString("Assets/Demo3/Shaders/DirectX/CubeShader.ps").c_str(),nullptr,ShaderLanguage::HLSL);


		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		API::VertexBufferDesc vDesc;
		vDesc.data = vertices;
		vDesc.size = sizeof(vertices);
		vDesc.usage = BufferGPUUsage::Dynamic;
		vDesc.accessflag = BufferCPUAccess::Default;
		CubeVB = new API::VertexBuffer(vDesc);

		CubeIL = new API::InputLayout();
		CubeIL->Push(0, "POSITION", API::DataType::Float3, 5 * sizeof(float), 0);
		CubeIL->Push(1, "TEXCOORD", API::DataType::Float2, 5 * sizeof(float), 3 * sizeof(float));

		CubeVB->SetInputLayout(CubeIL, CubeShader);
		
		CubeCB = new API::UniformBuffer("NECamera", sizeof(Shader_Uniforms));
		CubeShader->SetUniformBuffer(CubeCB, 0, ShaderType::Vertex);


		Texture_Desc Desc;
		Desc.Filter = TextureFilter::Trilinear;
		Desc.Wrap = TextureWrap::Repeat;
		Desc.Format = TextureFormat::R8G8B8A8;

		WoodenBoxTex = new API::Texture2D(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", Desc), Desc);

		Core::Context::EnableDepthBuffer(true);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}
	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.2f, 0.3f, 0.3f, 1.0f));

		WoodenBoxTex->Bind(0);
		CubeShader->Bind();
		CubeVB->Bind();


		Shader_Uniforms.Model = Math::Rotate(Math::Vector3(0.5f, 1.0f, 0.0f), 5.0f);
		Shader_Uniforms.View = Math::Translate(Math::Vector3(0.0f, 0.0f, -3.0f));
		Shader_Uniforms.Projection = Math::Perspective(Math::Deg2Rad(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

		CubeCB->Update(&Shader_Uniforms, sizeof(Shader_Uniforms));

		Core::Context::Draw(36);
		CubeVB->Unbind();
		CubeShader->Unbind();
		WoodenBoxTex->Unbind();

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{
		delete CubeShader;
		delete CubeVB;
		delete WoodenBoxTex;
	}
};
