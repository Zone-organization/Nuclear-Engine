#pragma once
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"

class SampleSelector : public Core::Game
{
	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
	}

	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		Graphics::Context::GetContext()->ClearRenderTarget(Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV(), ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(Graphics::Context::GetSwapChain()->GetDepthBufferDSV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		using namespace Graphics;
		ImGui::Begin("SampleSelector");

		ImGui::Text("Select Sample");

		if (ImGui::Button("Sample1 : BlinnPhong rendering of some models"))
		{
			ImGui::End();
			Core::Engine::EndFrame();
			Sample1 demo;
			return Core::Engine::RunGame(&demo);
		}
		if (ImGui::Button("Sample2 : Advanced Rendering "))
		{
			ImGui::End();
			Core::Engine::EndFrame();
			Sample2 demo;
			return Core::Engine::RunGame(&demo);
		}
		if (ImGui::Button("Sample3 : PhysX Integration"))
		{
			ImGui::End();
			Core::Engine::EndFrame();
			Sample3 demo;
			return Core::Engine::RunGame(&demo);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


	}
};