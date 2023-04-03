#pragma once
#include <Common.h>
#include <EditorUI.h>
using namespace Nuclear::Editor;
class NuclearEditor : public Core::Client
{
	
	EditorUI GUI;

public:
	NuclearEditor()
		:Client({"Nuclear Editor", "0.01 Alpha", "Zone - V7medZ", Core::ClientType::Editor }),
		GUI(this)
	{
	}
	Project mActiveProject;

	void OnMouseMovement(int xpos_a, int ypos_a) override
	{

	}
	void OnWindowResize(int width, int height) override
	{
		Graphics::GraphicsModule::Get().GetSwapChain()->Resize(width, height);

	}
	void Update(float deltatime) override
	{
		
	}
	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		auto* RTV = Graphics::GraphicsModule::Get().GetSwapChain()->GetCurrentBackBufferRTV();
		auto* DSV = Graphics::GraphicsModule::Get().GetSwapChain()->GetDepthBufferDSV();

		Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(1, &RTV, DSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(RTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::GraphicsModule::Get().GetContext()->ClearDepthStencil(DSV, Diligent::CLEAR_DEPTH_FLAG, 1.0f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		GUI.Render();
	}

	void Shutdown() override
	{
	}
};