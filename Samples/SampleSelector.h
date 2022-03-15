#pragma once
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"




class SampleSelector : public Core::Game
{
public:
	//static SampleSelector* Instance;
	// 
	//Asset Manager (Loader) for all samples
	Managers::AssetManager AssetLoader;

	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
	}
	SampleSelector()
	{
	//	Instance = this;
	}
	void StartSample()
	{
		ImGui::End();
		Core::Engine::EndFrame();

		Core::Engine::GetGame()->SetDefaultAssetManager(&AssetLoader);
		Core::Engine::LoadGame();
		Core::Engine::RunGame();
		Core::Engine::EndGame();

		if (Core::Engine::GetGame() != this)
		{
			Core::Engine::SetGame(this);
			Core::Engine::RunGame();
		}
	}

	void Load()
	{
		Assets::DefaultTextures::Initalize(&AssetLoader);
	}

	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
		auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();

		Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearRenderTarget(RTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		using namespace Graphics;
		ImGui::Begin("SampleSelector");

		ImGui::Text("Select Sample");

		if (ImGui::Button("Sample1 : BlinnPhong rendering of some models"))
		{
			Sample1 demo;
			Core::Engine::SetGame(&demo);
			return StartSample();
		}
		if (ImGui::Button("Sample2 : Advanced Rendering & PhysX"))
		{
			Sample2 demo;
			Core::Engine::SetGame(&demo);
			return StartSample();
		}
		if (ImGui::Button("Sample3 : PhysX Integration"))
		{
			Sample3 demo;
			Core::Engine::SetGame(&demo);
			return StartSample();
		}


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}
};