#pragma once
#include "Common.h"

class StellaTest : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
public:
	StellaTest()
	{
	}

	void Load()
	{
	
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	

	void Update(float deltatime) override
	{
		//Core::Application::SetTitle("Nuclear Engine FPS: " + std::to_string(FPS) + " FrameTime: " + std::to_string(FrameTime));

	}
	void Render(float dt) override
	{
		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		

		Graphics::API::Context::PresentFrame();
	}
};