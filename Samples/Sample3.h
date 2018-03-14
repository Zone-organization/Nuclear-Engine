#pragma once
#include "Common.h"

class Sample3 : public Core::Game
{
protected:
	//Assets
	API::Texture WhiteTex;



	//Default states
	API::CommonStates states;

public:
	Sample3()
	{
	}

	void SetupTextures()
	{
		API::Texture_Desc Desc;
		Desc.Format = API::Format::R8G8B8A8_UNORM;
		Desc.Type = API::TextureType::Texture2D;

		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);
	}

	void Load()
	{
	
		states.EnabledDepth_DisabledStencil.Bind();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void Render(float dt) override
	{
		Core::Context::Clear(API::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
	
		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);



		states.EnabledDepth_DisabledStencil.Bind();


		Core::Context::PresentFrame();
	}
};