#pragma once
#include "Common.h"

class BreakOut : public Core::Game
{
protected:
	Core::GameInfo Info;
	API::Texture *WoodenBoxTex;
	API::Shader *RendererShader;
	Renderers::SpriteRenderer *Renderer;

	Components::FlyCamera *Camera;
public:
	BreakOut()
	{
		Core::GameInfo Info;
		Info.Name = "Breakout";
		Info.Developer = "Zone Organization";
		Info.Version = "0.001";

		this->gameinfo = &Info;
	}
	void Load()
	{
		Camera = new Components::FlyCamera();
		Camera->Initialize(Math::Orthographic(0.0f, static_cast<float>(_Width_),static_cast<float>(_Height_), 0.0f, -1.0f, 1.0f));


		RendererShader = new API::Shader("SpriteRenderer",
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/SpriteRenderer.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/SpriteRenderer.ps.hlsl").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));
		
		Renderer = new Renderers::SpriteRenderer(RendererShader,Camera);


		Texture_Desc TexDesc;
		TexDesc.Filter = TextureFilter::Linear2D;
		TexDesc.Wrap = TextureWrap::Repeat;
		TexDesc.Format = TextureFormat::R8G8B8A8;
		TexDesc.Type = TextureType::Texture2D;
		WoodenBoxTex = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Common/Textures/woodenbox.jpg", TexDesc), TexDesc);

		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void PreRender(float deltatime) override
	{
	/*	if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::W))
			Camera->ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::A))
			Camera->ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::S))
			Camera->ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::D))
			Camera->ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);
*/
		Camera->Update();
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.1f, 0.1f, 0.1f, 1.0f));
		
		Renderer->DrawSprite(WoodenBoxTex, Math::Vector2(200.0f, 200.0f), (300, 400));

		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{		
		delete Camera;
	}
};