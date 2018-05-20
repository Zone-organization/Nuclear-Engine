//#pragma once
//#include "Common.h"
//
//class Sample3 : public Core::Game
//{
//protected:
//	//Assets
//	Assets::Texture BlockTex;
//	Components::Sprite *Block;
//	Systems::SpriteRenderSystem Renderer;
//	Components::CameraComponent Camera;
//
//	//Default states
//	Graphics::API::CommonStates states;
//
//public:
//	Sample3()
//	{
//	}
//
//	void Load()
//	{
//		Assets::Texture_Desc	TexDesc;
//		TexDesc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
//		TexDesc.Type = Assets::TextureUsageType::Texture2D;
//
//		BlockTex = Managers::AssetManager::Import("Assets/Breakout/Textures/block.png", TexDesc);
//		//Block = new Components::Sprite(&BlockTex, Math::vec2(200.0f, 200.0f), Math::vec2(300.0f, 400.0f), Graphics::Color(0.0f, 1.0f, 0.0f));
//
//		Camera.Initialize(Math::Orthographic(0.0f, static_cast<float>(_Width_), static_cast<float>(_Height_), 0.0f, -1.0f, 1.0f));
//
//		Systems::SpriteRenderSystemDesc Desc;
//		Core::Application::GetSize(Desc.Width, Desc.Height);
//		Desc.InitialCamera = &Camera;
//		Renderer.Initialize(Desc);
//
//		states.EnabledDepth_DisabledStencil.Bind();
//		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
//
//		//Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
//		Core::Application::Display();
//	}
//
//	void Render(float dt) override
//	{
//		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
//		Renderer.BeginDirectRender();
//		states.DefaultSampler.PSBind(0);
//		//Renderer.DirectRender(Block);
//
//
//		Camera.Update();
//
//		Graphics::API::Context::PresentFrame();
//	}
//};