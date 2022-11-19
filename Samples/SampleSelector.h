#pragma once
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include "SponzaDemo.h"
#include "Playground.h"

class SampleSelector : public Client
{
public:

	//Asset Manager (Loader) for all samples
	Managers::AssetManager AssetLoader;
	Managers::SceneManager SceneMgr;

	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
	}
	SampleSelector()
	{
		//	Instance = this;
	}
	void StartSample(Client* sample)
	{
		ImGui::End();
		Engine::GetInstance()->EndFrame();
		Engine::GetInstance()->EndClient();

		sample->SetDefaultAssetManager(&AssetLoader);
		sample->SetDefaultSceneManager(&SceneMgr);

		Engine::GetInstance()->LoadClient(sample);
		Engine::GetInstance()->EndClient();
	}

	void Load()
	{
		AssetLoader.Initialize();
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

		if (ImGui::Button("Sample1 : Basic model loading & animation & rendering"))
		{
			Sample1 demo;
			return StartSample(&demo);
		}
		/*if (ImGui::Button("Sample2 : Introduction to rendering pipelines and shading models."))
		{
			Sample2 demo;
			return StartSample(&demo);
		}*/
		//if (ImGui::Button("Sample3 : PhysX & Scripting Test ------OLD"))             //Improve
		//{
		//	Sample3 demo;
		//	return StartSample(&demo);
		//}

		//ImGui::Text("Select Demo");

		//if (ImGui::Button("Sponza Demo"))
		//{
		//	SponzaDemo demo;
		//	return StartSample(&demo);
		//}

		//if (ImGui::Button("Playground"))
		//{
		//Playground demo;
		//return StartSample(&demo);
		//}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
};