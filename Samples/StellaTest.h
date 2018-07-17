#pragma once
#include "Common.h"
#include "Experimental\Stella\Animator.h"
#include "Experimental\Stella\Layer.h"
#include "Experimental\Stella\Renderable.h"
#include "Experimental\Stella\Sprite.h"
#include "Experimental\Stella\Renderer.h"
#include "Experimental\Stella\SpriteSheet.h"

class StellaTest : public Core::Game
{
protected:

	//Graphics::Sprite sprite;
	Graphics::SpriteSheet spritesheet;
	Graphics::Layer layer;
	Graphics::Animator animator;
	Graphics::Renderable renderable;
	Graphics::Renderer renderer;

public:
	StellaTest()
	{
	}

	void Load()
	{
	


		LoadFinishUp();
	}

	

	void Update(float deltatime) override
	{


	}
	void Render(float dt) override
	{
		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		

		Graphics::API::Context::PresentFrame();
	}
};