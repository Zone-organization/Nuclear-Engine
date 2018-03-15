#pragma once
#include "Common.h"

class Sample3 : public Core::Game
{
protected:
	//Assets
	API::Texture BlockTex;
	Components::Sprite *Block;
	Systems::SpriteRenderSystem Renderer;
	Components::GenericCamera Camera;

	//Default states
	API::CommonStates states;

public:
	Sample3()
	{
	}

	void Load()
	{
		API::Texture_Desc	TexDesc;
		TexDesc.Format = API::Format::R8G8B8A8_UNORM;
		TexDesc.Type = API::TextureType::Texture2D;

		Managers::AssetManager::CreateTextureFromFile("Assets/Breakout/Textures/block.png", &BlockTex, TexDesc);
		Block = new Components::Sprite(&BlockTex, Math::Vector2(200.0f, 200.0f), Math::Vector2(300.0f, 400.0f));

		Camera.Initialize(Math::Orthographic(0.0f, static_cast<float>(_Width_), static_cast<float>(_Height_), 0.0f, -1.0f, 1.0f));

		Systems::SpriteRenderSystemDesc Desc;
		Core::Application::GetSize(Desc.Width, Desc.Height);
		Desc.InitialCamera = &Camera;
		Renderer.Initialize(Desc);

		states.EnabledDepth_DisabledStencil.Bind();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);

		//Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void Render(float dt) override
	{
		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer.BeginDirectRender();
		states.DefaultSampler.PSBind(0);
		Renderer.DirectRender(Block);


		Camera.Update();

		Core::Context::PresentFrame();
	}
};