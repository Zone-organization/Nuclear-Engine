#pragma once
#include <Common.h>
#include <EditorUI.h>
using namespace Nuclear::Editor;
class NuclearEditor : public Client
{
	
	EditorUI GUI;

public:
	NuclearEditor()
		:Client({"Nuclear Editor", "0.01 Alpha", "Zone - V7medZ", ClientType::Editor }),
		GUI(this)
	{
	}
	Project mActiveProject;

	void OnMouseMovement(int xpos_a, int ypos_a) override
	{

	}
	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);

	}
	void Update(float deltatime) override
	{
		
	}
	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
		auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();

		Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearRenderTarget(RTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		GUI.Render();
	}

	void Shutdown() override
	{
	}
};